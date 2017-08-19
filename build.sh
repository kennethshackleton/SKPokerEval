#!/bin/sh
./configure && cmake . && make && ctest --verbose
