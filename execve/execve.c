#include <dlfcn.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execve(const char *filename, char *const argv[], char *const envp[]) {
    void *libc = dlopen(LIBC, RTLD_LAZY);
    if (libc != NULL) {
        int (*realexec)(const char *filename, char *const argv[], char *const envp[]);
        *(void **) (&realexec) = dlsym(libc, "execve");
        if (dlerror() == NULL)  {
            char *root = getenv("OBSERVE_ROOT");
            char path[BUFSIZ];
            strcpy(path, root);
            strcat(path, "/handle");
            if (strcmp(filename, path) == 0) {
                // don't interrupt handle
                realexec(filename, argv, envp);
            } else {
                // insert filename into argv
                int argc = 0;
                while (argv[argc++] != NULL);
                char ** newargv = (char **)calloc(argc + 2, sizeof(char *));
                newargv[0] = filename;
                int i;
                for (i=0; i<argc; i++)
                    newargv[i+1] = argv[i];
                newargv[argc+1] = NULL;
                // pass everything to $OBSERVE_ROOT/handle
                strcpy(path, root);
                strcat(path, "/execve/hook");
                realexec(path, newargv, envp);
            }
        }
        dlclose(libc);
    }
    errno = ELIBACC;
    return -1;
}

