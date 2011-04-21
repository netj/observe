/*
 * release.c -- modify LD_PRELOAD and execvp with given arguments
 * 
 * Author: Jaeho Shin <netj@sparcs.org>
 * Created: 2008-04-09
 */

#include "observe.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

static char *const *prepare_argv(int argc, char *argv[]) {
    int i;
    char **newargv = (char **) malloc((argc - 1) * sizeof(char *));
    for (i=2; i<argc; i++)
        newargv[i - 2] = argv[i];
    newargv[argc - 2] = NULL;
    return newargv;
}

static void set_LD_PRELOAD(void) {
    char *preload = getenv("LD_PRELOAD");
    char *capturepath = getenv(OBSERVE_CAPTURE_SO_VAR);
    char *newpreload = NULL;
    if (preload == NULL) {
        newpreload = capturepath;
    } else {
        newpreload = (char *) malloc((
                    strlen(capturepath) + 1
                    + strlen(preload) + 1) * sizeof(char));
        sprintf(newpreload, "%s %s", capturepath, preload);
    }
    setenv("LD_PRELOAD", newpreload, 1);
}

int main(int argc, char *argv[]) {
    // prepare execvp's arguments from command line
    char *file = argv[1];
    char *const *newargv = prepare_argv(argc, argv);
    debugh(); debug("release(%s,", file); debugv(argv+2); debug(")\n");
    // add OBSERVE_CAPTURE_SO to LD_PRELOAD
    set_LD_PRELOAD();
    // let execvp handle the rest
    execvp(file, newargv);
    {
        // XXX: must not reach here
        char name[BUFSIZ];
        sprintf(name, "%s: %s", argv[0], argv[2]);
        perror(name);
    }
    return -1;
}
