#!/usr/bin/env bash

# detach OBSERVE_HOOKS_EXES from PATH
PATH=${PATH//$OBSERVE_HOOKS_EXES:/}
export PATH

# XXX: this is OK since calls with absolute path cannot be hooked
name=$1; shift
name=`cygpath -p "$name"`
name=`basename "$name"`

# search PATH
file=`type -P "$name"`

exec handle "$cmd" "$cmd" "$@"
