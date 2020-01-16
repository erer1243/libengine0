#!/bin/bash
[ -z "$1" ] && echo "usage: $0 [name]" && exit 1

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

printf "#include \"$1.h\"\n" > $DIR/src/$1.c
printf "#ifndef $1_h\n#define $1_h\n\n\n#endif" > $DIR/include/$1.h
# sed -i "/^\$(LIB):/ s/$/ obj\/$1.o/" $DIR/Makefile
