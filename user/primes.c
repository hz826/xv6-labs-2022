#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int pL[2], pR[2];

void prime(int p) {
    int x;
    int nxt = 0;

    printf("prime %d\n", p);
    while (read(pL[0], (void*)&x, 4)) {
        if (x % p == 0) continue;
        if (!nxt) {
            nxt = 1;
            pipe(pR);

            if (fork() == 0) {
                // printf("begin %d\n", x);
                close(pL[0]);
                pL[0] = pR[0];
                pL[1] = pR[1];
                close(pL[1]);
                prime(x);
            } else {
                close(pR[0]);
            }
        } else {
            write(pR[1], (void*)&x, 4);
        }
    }

    close(pL[0]);
    if (nxt) {
        close(pR[1]);
        wait(0);
    }

    // printf("end %d\n", x);
    exit(0);
}

int
main(int argc, char *argv[])
{
    int i;

    pipe(pR);

    if (fork() == 0) {
        pL[0] = pR[0];
        pL[1] = pR[1];
        close(pL[1]);
        prime(2);
    }

    close(pR[0]);

    for(i = 2; i <= 35; i++) {
        write(pR[1], (void*)&i, 4);
    }

    close(pR[1]);
    wait(0);
    exit(0);
}
