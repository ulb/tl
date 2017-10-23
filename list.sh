#!/usr/bin/env sh

cat atoms/$(($1-1))/all | ./run/inflate 2>/dev/null | ./run/dedup 2>/dev/null | ./run/sift 2>/dev/null atoms/$(($1-1))/all
