#include <unistd.h>
#include <stdio.h>

int main(void) {
    execl("/bin/ls", "ls", "-al", NULL);
    perror("execl");
    return -1;
}
