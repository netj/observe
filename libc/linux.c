#include "observe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

// delete given token from string
static char *strdel(char *str, const char *token, const char *delim) {
    char *last = str;
    char *original = strdup(str);
    char *p;
    char *saveptr;
    char *t;
    for (p = original; (t = strtok_r(p, delim, &saveptr)); p = NULL) {
        if (t == NULL)
            break;
        // skip token
        if (strcmp(token, t) == 0)
            continue;
        // insert delimiter if not first
        if (p == NULL)
            *last++ = ' ';
        // copy tokens
        strcpy(last, t);
        last += strlen(t);
    }
    free(original);
    *last = '\0';
    return str;
}

// remove OBSERVE_CAPTURE_SO from LD_PRELOAD
static char *const *reset_LD_PRELOAD(char *const envp[]) {
    const char *capturepath = getenv(OBSERVE_CAPTURE_SO_VAR);
    char **newenvp = NULL;
    if (envp != NULL) {
        int i;
        int j;
        int len;
        for (len=0; envp[len] != NULL; len++);
        j = len;
        newenvp = (char **) malloc((len + 1) * sizeof(char *));
        // copy envp to newenvp
        for (i=0,j=0; i<len; i++,j++) {
            newenvp[j] = strdup(envp[i]);
            if (strstr(envp[i], "LD_PRELOAD" "=") == envp[i]) {
                // modify LD_PRELOAD's value
                char *newpreload = strchr(newenvp[j], '=') + 1;
                strdel(newpreload, capturepath, " ");
                if (*newpreload == '\0') {
                    free(newenvp[j]);
                    j--;
                }
            }
        }
        newenvp[j] = NULL;
    } else {
        char *preload = getenv("LD_PRELOAD");
        if (preload != NULL) {
            char *newpreload = strdel(strdup(preload), capturepath, " ");
            if (*newpreload == '\0')
                unsetenv("LD_PRELOAD");
            else
                setenv("LD_PRELOAD", newpreload, 1);
        }
    }
    return newenvp;
}

// path to our handler program which disables observation state
static const char *observe_handler() {
    static char *handler = NULL;
    if (handler == NULL) {
        char *root = getenv(OBSERVE_ROOT_VAR);
        handler = (char *) malloc((strlen(root)
                    + sizeof(OBSERVE_HANDLER_PATH) + 1) * sizeof(char));
        sprintf(handler, "%s" OBSERVE_HANDLER_PATH, root);
    }
    return handler;
}

// insert handler name and file name into argv
static char *const *newargv(const char *file, char *const argv[]) {
    int i;
    int argc;
    const char **newargv;
    for (argc=0; argv[argc] != NULL; argc++);
    newargv = (const char **) malloc((argc + 3) * sizeof(char *));
    newargv[0] = OBSERVE_HANDLER_NAME;
    newargv[1] = strdup(file);
    for (i=0; i<argc; i++)
        newargv[i + 2] = strdup(argv[i]);
    newargv[argc + 2] = NULL;
    return (char *const *) newargv;
}

// dlsym symbol from dlopen'ed libc
static void **dlsym_libc(const char *symbol) {
    void *libc = dlopen(LIBC_PATH, RTLD_LOCAL | RTLD_NOW);
    if (libc != NULL) {
        void *sym = dlsym(libc, symbol);
        if (dlerror() == NULL)
            return sym;
        // close when error
        dlclose(libc);
    }
    debugh(); debug(" ! libc error\n");
    errno = ELIBACC;
    return NULL;
}


// execve hook
int execve(const char *path, char *const argv[], char *const envp[]) {
    int (*actual)(const char *path, char *const argv[], char *const envp[]);
    *(void **) (&actual) = dlsym_libc("execve");
    if (actual == NULL)
        return -1;
    debugh(); debug(" execve(%s,", path); debugv(argv); debug(")\n");
    // pass everything to our handler
    int r = actual(observe_handler(), newargv(path, argv), reset_LD_PRELOAD(envp));
    debugh(); debug(" ! execve error\n");
    return r;
}

// execvp hook
int execvp(const char *file, char *const argv[]) {
    int (*actual)(const char *file, char *const argv[]);
    *(void **) (&actual) = dlsym_libc("execvp");
    if (actual == NULL)
        return -1;
    debugh(); debug(" execvp(%s,", file); debugv(argv); debug(")\n");
    reset_LD_PRELOAD(NULL);
    int r = actual(observe_handler(), newargv(file, argv));
    debugh(); debug(" ! execvp error\n");
    return r;
}

// execv hook
int execv(const char *path, char *const argv[]) {
    int (*actual)(const char *path, char *const argv[]);
    *(void **) (&actual) = dlsym_libc("execv");
    if (actual == NULL)
        return -1;
    debugh(); debug(" execv (%s,", path); debugv(argv); debug(")\n");
    reset_LD_PRELOAD(NULL);
    int r = actual(observe_handler(), newargv(path, argv));
    debugh(); debug(" ! execv error\n");
    return r;
}

// TODO: execle hook
// TODO: execlp hook
// TODO: execl hook
