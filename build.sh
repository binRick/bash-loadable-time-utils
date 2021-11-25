#!/bin/bash
set -e
cd $(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)
td=$(mktemp -d)
of=$td/stdout
QTY=119
if ! command -v ansi >/dev/null; then
  alias ansi=$(pwd)/ansi
fi

if ! ( (./bootstrap.sh && ./configure && make clean && make) 2>&1 | pv -s $QTY -l -N "Compiling") >$of; then
	cat $of
else
	./test.sh
fi

rm -rf $td
