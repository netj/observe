#include "observe.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int i;
    char *file = argv[1];
    char **newargv = (char **) calloc(argc, sizeof(char *));
    for (i=2; i<argc; i++) {
        newargv[i-2] = argv[i];
        debug("%s ", argv[i]);
    }
    debug("\n");
    newargv[argc-1] = NULL;
    setenv(OBSERVE_FLAG_NAME, OBSERVE_FLAG_ENABLE, 1);
    execvp(file, newargv);
    perror(argv[0]);
    return -1;
}
