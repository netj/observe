int main(void) {
    int r;
    r = system("ls -al");
    if (r == -1)
        perror("system");
    return r;
}
