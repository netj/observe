#!/usr/bin/env bash

PATH=${PATH#$OBSERVE_HOOKS_EXES:}
PATH=${PATH%:$OBSERVE_HOOKS_EXES}
PATH=${PATH//:$OBSERVE_HOOKS_EXES:/:}

exec handle "$@"
