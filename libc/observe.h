/*
 * Vocabularies for observe's libc driver
 */

#define OBSERVE_ROOT_VAR        "OBSERVE_ROOT"
#define OBSERVE_CAPTURE_SO_VAR  "OBSERVE_CAPTURE_SO"

#define OBSERVE_HANDLER_NAME "handle"
#define OBSERVE_HANDLER_PATH "/" OBSERVE_HANDLER_NAME

#ifdef DEBUG
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define debug(arg...) fprintf(stderr, arg)
#define debugv(argv) {		\
    char **p = (char **)argv;	\
    while (*p != NULL)		\
	debug(" '%s'", *p), p++;  \
}
#define debugh() debug("## [%6d - %6d] ", getppid(), getpid())
#else
#define debug(arg...)
#define debugv(argv)
#define debugh()
#endif

#ifndef LIBC_PATH
#define LIBC_PATH NULL
#endif
