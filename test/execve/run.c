#include <unistd.h>
#include <stdio.h>

extern char **environ;

int main(void) {
    char *argv[] = {"ls", "-al", NULL};
    // TODO: modify environ and check later
    execve("/bin/ls", argv, environ);
    perror("execve");
    return -1;
}
