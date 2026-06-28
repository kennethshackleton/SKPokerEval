#!/usr/bin/env python
# -*- coding: utf-8 -*-

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

# Script for generating the first 13 non-negative integers with the property
# that the sum of any seven with each appearing at most four times is always
# unique among all such sums.
#
# The output is:
# [0, 1, 5, 22, 98, 453, 2031, 8698, 22854, 83661, 262349, 636345, 1479181].

prime = [0, 1, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
sums = [0]*1000000
rank = [0]*1000000
K = 3
while K <= 12:
    s = prime[K-1] + 1
    accept = False
    while not accept:
        t = 0
        accept = True
        prime[K] = s
        for i in xrange(0, K+1):
            for j in xrange(i, K+1):
                for k in xrange(j, K+1):
                    for l in xrange(k, K+1):
                        for m in xrange(l, K+1):
                            if m == i:
                                continue
                            for n in xrange(m, K+1):
                                if n == j:
                                    continue
                                for p in xrange(n, K+1):
                                    if p == k:
                                        continue
                                    sums[t] = prime[i] + prime[j] + prime[k] +\
                                        prime[l] + prime[m] + prime[n] +\
                                        prime[p]
                                    t += 1
        v = 0
        while accept and v < t-1:
            w = v + 1
            while accept and w < t:
                if sums[v] == sums[w]:
                    accept = False
                w += 1
            v += 1
        if accept:
            print prime
        else:
            s += 1
    K += 1
