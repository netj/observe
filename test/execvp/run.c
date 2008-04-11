#include <unistd.h>
#include <stdio.h>

int main(void) {
    char *argv[] = {"ls", "-al", NULL};
    execvp("ls", argv);
    perror("execvp");
    return -1;
}
