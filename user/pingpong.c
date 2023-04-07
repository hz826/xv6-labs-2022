#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int pid;
    int p0[2], p1[2];
    char s[2];

    pipe(p0);
    pipe(p1);

    if(fork() == 0) {
        pid = getpid();
        close(p0[1]);
        close(p1[0]);
        read(p0[0], s, 1);
        printf("%d: received p%sng\n", pid, s);
        write(p1[1], "o", 1);
    } else {
        pid = getpid();
        close(p0[0]);
        close(p1[1]);
        write(p0[1], "i", 1);
        read(p1[0], s, 1);
        printf("%d: received p%sng\n", pid, s);
    }
    exit(0);
}
