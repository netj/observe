#include "observe.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>

// determine state
static int observe_state(void) {
    char *enabled = getenv(OBSERVE_FLAG_NAME);
    if (enabled == NULL) {
        return OBSERVE_DISABLED;
    } else if (strncmp(enabled, OBSERVE_FLAG_ENABLED, 1) == 0) {
        return OBSERVE_ENABLED;
    } else {
        return OBSERVE_ENABLE;
    }
}

// enable observation state
static char *const *observe_enable(char *const envp[]) {
    char *const *p = envp;
    while (*p != NULL) {
        if (strstr(*p, OBSERVE_FLAG_NAME "=") == *p) {
            strcpy(*p + sizeof(OBSERVE_FLAG_NAME), OBSERVE_FLAG_ENABLED);
            return envp;
        }
        p++;
    }
    // FIXME: couldn't find OBSERVE_FLAG_NAME, must add one
    return envp;
}

// path to our escaper program which disables observation state
static const char *observe_handler() {
    static char *escaper = NULL;
    if (escaper == NULL) {
        char *root = getenv("OBSERVE_ROOT");
        escaper = (char *) malloc(sizeof(char) * (
                    strlen(root) + sizeof(OBSERVE_HANDLER_PATH)));
        sprintf(escaper, "%s" OBSERVE_HANDLER_PATH, root);
    }
    return escaper;
}

// insert handler name and file name into argv
static char *const *newargv(const char *file, char *const argv[]) {
    int i;
    int argc;
    const char **newargv;
    for (argc=0; argv[argc] != NULL; argc++);
    newargv = (const char **) calloc(argc + 3, sizeof(char *));
    newargv[0] = OBSERVE_HANDLER_NAME;
    newargv[1] = file;
    for (i=0; i<argc; i++)
        newargv[i+2] = argv[i];
    newargv[argc+2] = NULL;
    return (char *const *) newargv;
}

// dlsym symbol from dlopen'ed libc
static void **dlsym_libc(const char *symbol) {
    void *libc = dlopen(LIBC_PATH, RTLD_LAZY);
    if (libc != NULL) {
        void *sym = dlsym(libc, symbol);
        if (dlerror() == NULL)
            return sym;
        // close when error
        dlclose(libc);
    }
    errno = ELIBACC;
    return NULL;
}


// execve(2) hook
int execve(const char *filename, char *const argv[], char *const envp[]) {
    int (*actual)(const char *filename, char *const argv[], char *const envp[]);
    *(void **) (&actual) = dlsym_libc("execve");
    if (actual == NULL)
        return -1;
    debug("# execve(2) # %s %s\n", getenv(OBSERVE_FLAG_NAME), filename);
    switch (observe_state()) {
        case OBSERVE_ENABLED:   // pass everything to handler
            return actual(observe_handler(), newargv(filename, argv), envp);
        case OBSERVE_DISABLED:  // let actual handle
            return actual(filename, argv, envp);
        case OBSERVE_ENABLE:    // let actual handle with observation enabled
            return actual(filename, argv, observe_enable(envp));
    }
    return -1;
}

// TODO: execv(3) hook

// execvp(3) hook
int execvp(const char *file, char *const argv[]) {
    int (*actual)(const char *file, char *const argv[]);
    *(void **) (&actual) = dlsym_libc("execvp");
    if (actual == NULL)
        return -1;
    debug("# execvp(3) # %s %s %s\n", getenv(OBSERVE_FLAG_NAME), file, argv[0]);
    switch (observe_state()) {
        case OBSERVE_ENABLED:   // pass everything to handler
            return actual(observe_handler(), newargv(file, argv));
        case OBSERVE_DISABLED:  // let actual handle
            return actual(file, argv);
        case OBSERVE_ENABLE:    // let actual handle with observation enabled
            setenv(OBSERVE_FLAG_NAME, OBSERVE_FLAG_ENABLED, 1);
            return actual(file, argv);
    }
    return -1;
}

// TODO: execl(3) hook
// TODO: execlp(3) hook
// TODO: execle(3) hook
