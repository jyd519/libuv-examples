#include <unistd.h>
#include <stdio.h>

int main() {
    printf("child starting\n");
    fflush(stdout);
    sleep(1);
    printf("child running\n");
    fflush(stdout);
    sleep(2);
    printf("child ending\n");
    fflush(stdout);
    return 0;
}
