#define OBSERVE_HANDLER_NAME "escape"
#define OBSERVE_HANDLER_PATH "/libc/" OBSERVE_HANDLER_NAME

enum { OBSERVE_DISABLED, OBSERVE_ENABLED, OBSERVE_ENABLE };
#define OBSERVE_FLAG_NAME "OBSERVE_ENABLED"
#define OBSERVE_FLAG_DISABLED NULL
#define OBSERVE_FLAG_ENABLED  "1"
#define OBSERVE_FLAG_ENABLE   "0"

#ifdef DEBUG
#include <stdio.h>
#define debug(arg...) fprintf(stderr, arg)
#else
#define debug(arg...)
#endif

#ifndef LIBC_PATH
#define LIBC_PATH NULL
#endif
