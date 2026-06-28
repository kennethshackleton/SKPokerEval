#!/usr/bin/env python3

# SKPokerEval
#
# Copyright 2010 Kenneth J. Shackleton
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
A Czech-Havas-Majewski perfect hash implementation.
See "Fundamental Study Perfect Hashing" by Czech, Havas, Majewski,
Theoretical Computer Science 182 (1997) 1-143.
"""

import argparse
import json
import os
import sys
import time
from typing import Dict, List

import numpy as np

from rank import rank


FACE_VALUES = [0, 1, 5, 22, 98, 453, 2031, 8698, 22854, 83661, 262349, 636345, 1479181]
SHIFT = 9
MULTIPLIER = 6019237
BITS = 23
FACE_BIT_MASK = (1 << BITS) - 1


def build_key_rank_map() -> Dict[int, int]:
    """
    Generate every unique 7-card face-sum and map it to its poker rank.
    Matches the exact nested-loop structure from perfect_hash.py.
    """
    val = FACE_VALUES
    key_rank: Dict[int, int] = {}
    for i in range(1, 13):
        for j in range(1, i + 1):
            for k in range(1, j + 1):
                for l in range(0, k + 1):
                    for m in range(0, l + 1):
                        if not m < i:
                            break
                        for n in range(0, m + 1):
                            if not n < j:
                                break
                            for p in range(0, n + 1):
                                if not p < k:
                                    break
                                key = (val[i] + val[j] + val[k] +
                                       val[l] + val[m] + val[n] + val[p])
                                if key not in key_rank:
                                    key_rank[key] = rank(key)
    return key_rank


NOT_A_VALUE = -1


def build_chm(
    key_rank: Dict[int, int],
    shift: int,
    multiplier: int,
    verbose: bool = True,
):
    """
    Build the two CHM arrays (offsets[], rank_hash[]) for a given SHIFT.

    The hash is:  h = multiplier * key & FACE_BIT_MASK
    Row index:    h >> shift
    Column index: h & ((1 << shift) - 1)

    Returns (offsets, rank_hash) as Python lists of ints.
    The rank_hash list has no -1 sentinels, unused slots are 0.
    """
    side = 1 << shift
    col_mask = side - 1
    num_rows = 1 << (BITS - shift)

    if verbose:
        print(f"  shift={shift}: side={side} cols, {num_rows} rows, multiplier={multiplier}")

    diffused: Dict[int, int] = {}
    for key, r in key_rank.items():
        h = multiplier * key & FACE_BIT_MASK
        if h in diffused:
            assert diffused[h] == r, (
                f"Hash collision with different ranks: h={h}, "
                f"ranks {diffused[h]} vs {r}"
            )
        else:
            diffused[h] = r

    if verbose:
        print(f"  {len(diffused)} unique hashes (expected {len(key_rank)})")

    offsets = [0] * num_rows
    HASH_TABLE_CAP = max(len(key_rank) + side + 1024, 8192)
    MAX_HASH_TABLE_CAP = 300_000
    ht = np.full(MAX_HASH_TABLE_CAP + side, NOT_A_VALUE, dtype=np.int32)
    length = 0

    row_map: dict = {}
    for h, r in diffused.items():
        row = h >> shift
        col = h & col_mask
        row_map.setdefault(row, []).append((col, r))

    row_active = sorted(row_map.items(), key=lambda x: len(x[1]), reverse=True)

    t0 = time.time()
    done = 0
    for row_idx, active in row_active:
        done += 1
        if verbose and done % 2000 == 0:
            print(f"    placed {done}/{len(row_active)}  length={length}  "
                  f"elapsed={time.time()-t0:.1f}s")

        any_bad = np.zeros(HASH_TABLE_CAP, dtype=bool)
        for col, rv in active:
            slot = ht[col: col + HASH_TABLE_CAP]
            any_bad |= (slot != NOT_A_VALUE) & (slot != rv)

        safe = np.flatnonzero(~any_bad)
        if safe.size == 0:
            raise RuntimeError(
                f"Could not place row {row_idx} within {HASH_TABLE_CAP} slots — "
                "increase MAX_HASH_TABLE_CAP"
            )
        j = int(safe[0])

        offsets[row_idx] = j
        for col, s in active:
            pos = j + col
            ht[pos] = s
            if pos + 1 > length:
                length = pos + 1

        HASH_TABLE_CAP = min(length + side + 2048, MAX_HASH_TABLE_CAP)

    result_arr = ht[:length].copy()
    result_arr[result_arr == NOT_A_VALUE] = 0
    result = result_arr.tolist()

    if verbose:
        print(f"  CHM done: offsets[{num_rows}] + rank_hash[{length}]")
        print(f"  Total bytes: {(num_rows + length) * 2:,}")
        print(f"  Elapsed: {time.time()-t0:.1f}s")
    return offsets, result


def verify(
    key_rank: Dict[int, int],
    offsets: List[int],
    hash_table: List[int],
    shift: int,
    multiplier: int,
) -> bool:
    """Check every key is accociated with the correct rank."""
    side = 1 << shift
    col_mask = side - 1
    errors = 0
    for key, expected in key_rank.items():
        h = multiplier * key & FACE_BIT_MASK
        idx = offsets[h >> shift] + (h & col_mask)
        got = hash_table[idx]
        if got != expected:
            print(f"  FAIL key={key} h={h} idx={idx} got={got} expected={expected}")
            errors += 1
            if errors > 10:
                print("  (too many errors, stopping)")
                break
    if errors:
        print(f"Verification FAILED ({errors} errors)")
    else:
        print(f"Verification passed for all {len(key_rank)} keys.")
    return errors == 0


def write_separate_headers(
    offsets: List[int],
    hash_table: List[int],
    shift: int,
    multiplier: int,
    out_dir: str,
):
    """
    Write RankOffsets.h and RankHash.h as separate files matching the
    original layout used by SevenEval.h (so no C++ changes are needed).
    """
    side = 1 << shift
    col_mask = side - 1
    max_offset = max(offsets)
    off_type = "uint16_t" if max_offset <= 65535 else "uint32_t"
    COLS = 16
    os.makedirs(out_dir, exist_ok=True)

    lines = """
// SKPokerEval
//
// Copyright 2010 Kenneth J. Shackleton
//
// This program gives you software freedom; you can copy, convey, propagate,
// redistribute and/or modify this program under the terms of the GNU General
// Public License (GPL) as published by the Free Software Foundation (FSF),
// either version 3 of the License, or (at your option) any later version of
// the GPL published by the FSF.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program in a file in the top-level directory called "GPLv3". If
// not, see http://www.gnu.org/licenses/.
""".splitlines()
    lines.append("")
    lines.append("#include <cstdint>")
    lines.append(f"{off_type} const offsets[{len(offsets)}] = {{")
    for i in range(0, len(offsets), COLS):
        chunk = offsets[i:i + COLS]
        lines.append("  " + ", ".join(str(x) for x in chunk) + ",")
    lines[-1] = lines[-1].rstrip(",")
    lines.append("};")
    path = os.path.join(out_dir, "RankOffsets.h")
    with open(path, "w") as fh:
        fh.write("\n".join(lines) + "\n")
    print(f"Written: {path}")

    lines = []
    lines.append("#include <cstdint>")
    lines.append(f"uint16_t const rank_hash[{len(hash_table)}] = {{")
    for i in range(0, len(hash_table), COLS):
        chunk = hash_table[i:i + COLS]
        lines.append("  " + ", ".join(str(x) for x in chunk) + ",")
    lines[-1] = lines[-1].rstrip(",")
    lines.append("};")
    path = os.path.join(out_dir, "RankHash.h")
    with open(path, "w") as fh:
        fh.write("\n".join(lines) + "\n")
    print(f"Written: {path}")

    print(f"")
    print(f"Update src/Constants.h:")
    print(f"  RANK_OFFSET_SHIFT = {shift}")
    print(f"  RANK_HASH_MOD     = {col_mask}u")
    if multiplier != MULTIPLIER:
        print(f"  (also update MULTIPLIER in SevenEval.h to {multiplier})")


STATE_FILE = "generate_rank_tables_state.json"


def save_state(shift: int, offsets: List[int], hash_table: List[int]):
    tmp = STATE_FILE + ".tmp"
    with open(tmp, "w") as fh:
        json.dump({"shift": shift, "offsets": offsets, "hash_table": hash_table},
                  fh)
    os.replace(tmp, STATE_FILE)
    print(f"State saved to {STATE_FILE}")


def load_state():
    if not os.path.exists(STATE_FILE):
        return None
    with open(STATE_FILE) as fh:
        return json.load(fh)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--shift", type=int, default=SHIFT,
                        help=f"RANK_OFFSET_SHIFT (default {SHIFT})")
    parser.add_argument("--multiplier", type=int, default=MULTIPLIER,
                        help=f"Hash multiplier (default {MULTIPLIER})")
    parser.add_argument("--out-dir", default=None,
                        help="Output directory (default: ../src)")
    parser.add_argument("--resume", action="store_true",
                        help="Resume from checkpoint if available")
    args = parser.parse_args()

    shift = args.shift
    multiplier = args.multiplier
    out_dir = args.out_dir or os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "..", "src"
    )

    print("=" * 60)
    print(f"SKPokerEval CHM rank table generator  shift={shift}  M={multiplier}")
    print("=" * 60)

    if args.resume:
        state = load_state()
        if state and state["shift"] == shift:
            print("Resuming from checkpoint...")
            offsets = state["offsets"]
            hash_table = state["hash_table"]
            print(f"  Loaded offsets[{len(offsets)}] + rank_hash[{len(hash_table)}]")
            print("Building key→rank map for verification...")
            key_rank = build_key_rank_map()
            if verify(key_rank, offsets, hash_table, shift):
                write_separate_headers(offsets, hash_table, shift, out_dir)
            return

    print("Building key→rank map...")
    t0 = time.time()
    key_rank = build_key_rank_map()
    print(f"  {len(key_rank)} unique keys in {time.time()-t0:.2f}s")

    print("Running CHM algorithm...")
    offsets, hash_table = build_chm(key_rank, shift, multiplier)

    save_state(shift, offsets, hash_table)

    print("Verifying...")
    if not verify(key_rank, offsets, hash_table, shift, multiplier):
        print("ERROR: verification failed — tables NOT written.")
        sys.exit(1)

    write_separate_headers(offsets, hash_table, shift, multiplier, out_dir)

    print()
    print("=== Summary ===")
    print(f"  offsets[{len(offsets)}]      = {len(offsets)*2:,} bytes")
    print(f"  rank_hash[{len(hash_table)}]    = {len(hash_table)*2:,} bytes")
    print(f"  Total               = {(len(offsets)+len(hash_table))*2:,} bytes")


if __name__ == "__main__":
    main()
