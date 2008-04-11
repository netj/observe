#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main(void) {
    execle("/bin/ls", "ls", "-al", NULL, environ);
    perror("execle");
    return -1;
}
