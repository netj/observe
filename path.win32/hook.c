/*
 * observe cygwin hook wrapper
 * Author: Jaeho Shin <netj@sparcs.org>
 * Created: 2008-04-08
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CMDFMT "sh.exe -c \"'%s/path.win32/hook.sh'"

char *quoteapos(const char *s) {
    // TODO
    return NULL;
}

int main(int argc, char* argv[])
{
    char* cmdln;
    char* p;
    int i;
    char *root = getenv("OBSERVE_ROOT");
    int len = 0;
    if (root == NULL) {
        fprintf(stderr, "OBSERVE_ROOT undefined\n");
        return 1;
    }
    len += strlen(root);
    len += sizeof(CMDFMT);
    for (i=0;i<argc;i++)
        len += strlen(argv[i]) + 3;
    cmdln = (char*)calloc(len + 1, sizeof(char));
    sprintf(cmdln, CMDFMT, root);
    p = cmdln + strlen(cmdln);
    for (i=0;i<argc;i++) {
        *p++ = ' ';
        *p++ = '\'';
        len = strlen(argv[i]);
        strncpy(p, argv[i], len);
        // FIXME escape ' and "s
        p += len;
        *p++ = '\'';
    }
    strcat(p, "\"");
    //fprintf(stderr, "######## %s\n", cmdln);
    return system(cmdln);
}
