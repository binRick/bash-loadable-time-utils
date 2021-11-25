#!/bin/bash
set -e
DEFAULT_CMD="./test.sh"
cmd="${@:-$DEFAULT_CMD}"
flock_pfx="flock -x /var/spool/bash-loadables.lock"
cmd="passh reap nodemon -i src/\*.lo -i src/\*.la  -w \*.sh -w src -e c,sh,h --delay .2 -V -x /bin/sh -- -c '$flock_pfx sh -c \"make clean >/dev/null 2>&1; make >/dev/null && $cmd\"||true'"

eval "$cmd"
