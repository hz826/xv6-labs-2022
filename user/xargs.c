#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

char getc() {
    static char buf[512];
    static int i = 0, n = 0;

    if (i == n) {
        n = read(0, buf, sizeof buf);
        if(n == 0)
            return '\0';
        if(n < 0){
            fprintf(2, "read error\n");
            exit(0);
        }
        i = 0;
    }
    return buf[i++];
}

void getline(char *buf) {
    char *c;
    c = buf;
    while (1) {
        *c=getc();
        if (*c == '\n' || *c == '\0') break;
        c++;
    }
    *c = '\0';
}

int
main(int argc, char *argv[])
{
    int i;
    char *argv2[MAXARG];
    char buf[1024];

    if (argc < 1) exit(0);

    while (1) {
        getline(buf);
        if (*buf == '\0') break;

        argv2[0] = argv[1];
        for (i = 2; i < argc; i++) argv2[i-1] = argv[i];
        argv2[argc-1] = buf;
        argv2[argc] = 0;

        if (fork() == 0) {
            exec(argv2[0], argv2);
            exit(0);
        } else {
            wait(0);
        }
    }

    exit(0);
}
