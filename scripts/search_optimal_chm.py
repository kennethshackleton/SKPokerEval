#!/usr/bin/env python3

# SKPokerEval
#
# Copyright 2026 Kenneth J. Shackleton
#
# This program gives you software freedom; you can copy, convey, propagate,
# redistribute and/or modify this program under the terms of the GNU General
# Public License (GPL) as published by the Free Software Foundation (FSF),
# either version 3 of the License, or (at your option) any later version of
# the GPL published by the FSF.
#
# This program is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
# or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
# for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program in a file in the top-level directory called "GPLv3". If
# not, see http://www.gnu.org/licenses/.

"""
Usage
-----
    python3 search_optimal_chm.py [--shifts 9] [--top-k 500]
    python3 search_optimal_chm.py --resume
"""

import argparse
import heapq
import json
import os
import sys
import time
from multiprocessing import Pool, cpu_count
from typing import Dict

import numpy as np

FACE_VALUES = [0, 1, 5, 22, 98, 453, 2031, 8698, 22854, 83661, 262349, 636345, 1479181]
FACE_BIT_MASK = (1 << 23) - 1
BITS = 23
MAX_M = 1 << BITS

SCREEN_CHUNK = 10_000

BEST_KNOWN = {"shift": 9, "M": 6019237, "total_bytes": 102_426, "rh_len": 34_829}

_HERE = os.path.dirname(os.path.abspath(__file__))
STATE_FILE = os.path.join(_HERE, "search_optimal_state.json")

_KEYS: np.ndarray = None
_KEY_RANK: dict = None


def _init_globals(keys_bytes: bytes, key_rank_items: list):
    global _KEYS, _KEY_RANK
    _KEYS = np.frombuffer(keys_bytes, dtype=np.int64).copy()
    _KEY_RANK = dict(key_rank_items)
    if _HERE not in sys.path:
        sys.path.insert(0, _HERE)


def _screen_proxy_worker(args):
    """
    Compute sum_spans and max_occ proxy metrics for all odd M in [m_start, m_end).

    Returns list of [sum_spans, max_occ, M].
    """
    shift, m_start, m_end = args
    keys = _KEYS
    N = len(keys)

    if m_start % 2 == 0:
        m_start += 1

    results = []
    for m in range(m_start, m_end, 2):
        h = ((np.int64(m) * keys) & FACE_BIT_MASK).astype(np.int32)
        h.sort()

        rows_s = h >> shift
        bounds = np.flatnonzero(np.diff(rows_s))
        g_starts = np.concatenate([[0], bounds + 1])
        g_ends = np.concatenate([bounds, [N - 1]])

        sum_sp  = int((h[g_ends] - h[g_starts] + 1).sum())
        max_occ = int((g_ends - g_starts + 1).max())

        results.append([sum_sp, max_occ, m])

    return results


def _chm_worker(args):
    """Run full CHM for one (shift, M) pair.  Uses global _KEY_RANK."""
    shift, m = args
    from generate_rank_tables import build_chm
    try:
        offsets, ht = build_chm(_KEY_RANK, shift, m, verbose=False)
        total = (len(offsets) + len(ht)) * 2
        return (shift, m, total, len(ht))
    except Exception as exc:
        return (shift, m, -1, str(exc))


def build_key_rank_map() -> Dict[int, int]:
    sys.path.insert(0, _HERE)
    from rank import rank
    val = FACE_VALUES
    kr: Dict[int, int] = {}
    for i in range(1, 13):
      for j in range(1, i+1):
        for k in range(1, j+1):
          for l in range(0, k+1):
            for m in range(0, l+1):
              if not m < i: break
              for n in range(0, m+1):
                if not n < j: break
                for p in range(0, n+1):
                  if not p < k: break
                  key = val[i]+val[j]+val[k]+val[l]+val[m]+val[n]+val[p]
                  if key not in kr:
                      kr[key] = rank(key)
    return kr


def topk_add(heap: list, item: list, k: int):
    neg = (-item[0], -item[1], item[2])
    if len(heap) < k:
        heapq.heappush(heap, neg)
    elif item[0] < -heap[0][0]:
        heapq.heapreplace(heap, neg)


def topk_sorted(heap: list) -> list:
    """Return [[sum_sp, max_occ, M], ...] sorted ascending by sum_sp."""
    return sorted([[-h[0], -h[1], h[2]] for h in heap])


def save_state(state: dict):
    tmp = STATE_FILE + ".tmp"
    with open(tmp, "w") as fh:
        json.dump(state, fh, indent=2)
    os.replace(tmp, STATE_FILE)


def load_state() -> dict:
    if not os.path.exists(STATE_FILE):
        return None
    with open(STATE_FILE) as fh:
        return json.load(fh)


def make_shift_state() -> dict:
    return {
        "screen_done": False,
        "screen_next_m": 1,
        "topk_heap": [],
        "chm_done_set": [],
        "chm_results": [],
    }


def run_screening(state: dict, shift: int, pool: Pool, top_k: int):
    ss = state["shifts"][str(shift)]
    if ss["screen_done"]:
        print(f"  shift={shift}: screening already complete "
              f"({len(ss['topk_heap'])} candidates in heap).")
        return

    next_m = ss["screen_next_m"]
    chunks = [
        (shift, m, min(m + SCREEN_CHUNK, MAX_M))
        for m in range(next_m, MAX_M, SCREEN_CHUNK)
    ]
    total_odd = (MAX_M - next_m + 1) // 2
    print(f"  shift={shift}: screening ~{total_odd:,} odd M values "
          f"({len(chunks)} tasks) …")

    heap = ss["topk_heap"]
    heapq.heapify(heap)
    valid_total = 0
    t0 = time.time()

    for ci, result in enumerate(pool.imap(_screen_proxy_worker, chunks, chunksize=1)):
        valid_total += len(result)
        for item in result:
            topk_add(heap, item, top_k)

        ss["screen_next_m"] = chunks[ci][2]
        ss["topk_heap"] = heap
        save_state(state)

        if (ci + 1) % 10 == 0 or ci == len(chunks) - 1:
            elapsed = time.time() - t0
            done_m = (chunks[ci][2] - next_m + 1) // 2
            rate = done_m / elapsed if elapsed else 1
            best_ss = -heap[0][0] if heap else "—"
            print(f"    chunk {ci+1}/{len(chunks)}  valid={valid_total:,}  "
                  f"heap_size={len(heap)}  best_sum_spans={best_ss}  "
                  f"{rate/1000:.0f}K M/s  {elapsed:.0f}s")

    ss["screen_done"] = True
    ss["topk_heap"]   = heap
    save_state(state)

    best5 = topk_sorted(heap)[:5]
    print(f"  Screening complete.  {valid_total:,} valid M found.")
    print(f"  Top-5 proxy candidates: {best5}")


def run_chm_phase(state: dict, shift: int, pool: Pool):
    ss = state["shifts"][str(shift)]
    heap = ss["topk_heap"]
    heapq.heapify(heap)

    candidates = topk_sorted(heap)
    done_set = set(ss["chm_done_set"])
    todo = [(shift, item[2]) for item in candidates if item[2] not in done_set]

    if not todo:
        print(f"  shift={shift}: CHM already complete.")
        return

    print(f"  shift={shift}: running CHM on {len(todo)} candidates …")
    t0 = time.time()
    done = 0

    for shift_r, m, total, rh_or_err in pool.imap_unordered(_chm_worker, todo, chunksize=1):
        done += 1
        ss["chm_done_set"].append(m)

        if isinstance(total, int) and total > 0:
            ss["chm_results"].append([m, total, rh_or_err])
            if total < state["best"]["total_bytes"]:
                state["best"] = {
                    "shift": shift_r, "M": m,
                    "total_bytes": total, "rh_len": rh_or_err,
                }
                print(f"\n  *** NEW BEST: shift={shift_r}  M={m}  "
                      f"total={total:,}B  rh_len={rh_or_err} ***\n")

        if done % 20 == 0 or done == len(todo):
            elapsed = time.time() - t0
            rate    = done / elapsed if elapsed else 0
            print(f"    {done}/{len(todo)}  best={state['best']['total_bytes']:,}B  "
                  f"{rate:.2f} M/s  {elapsed:.0f}s")

        save_state(state)

    results = sorted(ss["chm_results"], key=lambda x: x[1])[:10]
    print(f"  shift={shift}: top-10 results:")
    for m, total, rh_len in results:
        print(f"    M={m:>10}  total={total:,}B  rh_len={rh_len}")


def main():
    sys.stdout.reconfigure(line_buffering=True)

    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--shifts", type=int, nargs="+", default=[9],
                    help="Shift values to search (default: 9)")
    ap.add_argument("--top-k", type=int, default=500,
                    help="CHM candidates per shift (default: 500)")
    ap.add_argument("--workers", type=int, default=None,
                    help="Worker processes (default: cpu_count)")
    ap.add_argument("--resume", action="store_true",
                    help="Resume from search_optimal_state.json")
    args = ap.parse_args()

    n_workers = args.workers or cpu_count()

    print("=" * 62)
    print(f"SKPokerEval CHM search  shifts={args.shifts}  "
          f"top-k={args.top_k}  workers={n_workers}")
    print(f"  MAX_M={MAX_M:,}  SCREEN_CHUNK={SCREEN_CHUNK:,}")
    print(f"  Best known: shift={BEST_KNOWN['shift']}  M={BEST_KNOWN['M']}  "
          f"total={BEST_KNOWN['total_bytes']:,}B")
    print("=" * 62)

    if args.resume:
        state = load_state() or {}
        state.setdefault("best", dict(BEST_KNOWN))
        state.setdefault("shifts", {})
        print(f"Resumed.  Current best: {state['best']}")
    else:
        state = {"best": dict(BEST_KNOWN), "shifts": {}}
        save_state(state)

    for shift in args.shifts:
        if str(shift) not in state["shifts"]:
            state["shifts"][str(shift)] = make_shift_state()

    print("\nBuilding key→rank map …")
    t0 = time.time()
    key_rank = build_key_rank_map()
    print(f"  {len(key_rank)} keys in {time.time()-t0:.2f}s")

    keys_arr = np.array(sorted(key_rank.keys()), dtype=np.int64)
    keys_bytes = keys_arr.tobytes()

    with Pool(processes=n_workers,
              initializer=_init_globals,
              initargs=(keys_bytes, list(key_rank.items()))) as pool:
        for shift in args.shifts:
            print(f"\n{'─' * 50}")
            print(f"  SHIFT = {shift}")
            print(f"{'─' * 50}")
            run_screening(state, shift, pool, args.top_k)
            run_chm_phase(state, shift, pool)

    print("\n" + "=" * 62)
    print("FINAL BEST:")
    b = state["best"]
    orig = 116_922
    print(f"  shift={b['shift']}  M={b['M']}  "
          f"total={b['total_bytes']:,}B  rh_len={b['rh_len']}")
    print(f"  vs original (M=31): delta={b['total_bytes'] - orig:+,}B "
          f"({(b['total_bytes'] - orig) / orig * 100:+.1f}%)")


if __name__ == "__main__":
    main()
