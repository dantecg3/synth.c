#!/usr/bin/env bash

set -e

CC="gcc"
STD="-std=c99"
OPT="-O0"

SRC=""
OUT="a.out"

while [[ $# -gt 0 ]]; do
    case "$1" in
        -o)
            OUT="$2"
            shift 2
            ;;
        -O0|-O1|-O2|-O3|-Os)
            OPT="$1"
            shift
            ;;
        *)
            SRC="$1"
            shift
            ;;
    esac
done

if [[ -z "$SRC" ]]; then
    echo "Usage: mycc <source.c> -o <output>"
    exit 1
fi

INCLUDES="-Isrc"

echo "$CC $SRC $INCLUDES -o $OUT $STD $OPT -Wall -Wextra -lm"
exec $CC "$SRC" $INCLUDES -o "$OUT" $STD $OPT -Wall -Wextra -lm