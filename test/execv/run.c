#include <unistd.h>
#include <stdio.h>

int main(void) {
    char *argv[] = {"ls", "-al", NULL};
    execv("/bin/ls", argv);
    perror("execv");
    return -1;
}
