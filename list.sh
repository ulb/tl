#!/usr/bin/env sh

cat db/src/$(($1-1)) |
./run/inflate 2>/dev/null |
./run/dedup 2>/dev/null |
./run/sift 2>/dev/null db/src/$(($1-1))
