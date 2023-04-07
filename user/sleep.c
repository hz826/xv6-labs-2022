#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int t, i;

  if(argc <= 1){
    fprintf(2, "sleep: need an argument\n");
    exit(0);
  }

  for(i = 1; i < argc; i++){
    t = atoi(argv[i]);
    sleep(t);
  }
  exit(0);
}