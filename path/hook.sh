#!/usr/bin/env bash

PATH=${PATH#$OBSERVE_HOOKS_EXES:}
PATH=${PATH%:$OBSERVE_HOOKS_EXES}
PATH=${PATH//:$OBSERVE_HOOKS_EXES:/:}

# XXX: this is OK since calls with absolute path cannot be hooked
cmd=`basename "$0"`

exec handle "$cmd" "$@"
