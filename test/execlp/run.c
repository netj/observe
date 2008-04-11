#include <unistd.h>
#include <stdio.h>

int main(void) {
    execlp("ls", "ls", "-al", NULL);
    perror("execlp");
    return -1;
}
