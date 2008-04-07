#include <string.h>
#include <errno.h>

#define HOOKCMD "hook"

char *quoteapos(const char *s) {
    // TODO
    return NULL;
}

int main(int argc, char* argv[])
{
    char* cmdln;
    char* p;
    int i, len = strlen(HOOKCMD);
    for (i=0;i<argc;i++)
        len += strlen(argv[i]) + 3;
    cmdln = (char*)calloc(len + 1, sizeof(char));
    strcpy(cmdln, HOOKCMD);
    p = cmdln + strlen(HOOKCMD);
    for (i=0;i<argc;i++) {
        *p++ = ' ';
        *p++ = '"';
        len = strlen(argv[i]);
        strncpy(p, argv[i], len);
        // TODO
        p += len;
        *p = '"';
    }
    return system(cmdln);
}
