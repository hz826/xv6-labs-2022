#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *dest)
{
  char buf[512], *p, *pp;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_DEVICE:
  case T_FILE:
    p = pp = path;
    while (*pp != '\0') {
      if (*pp == '/') p = pp+1;
      ++pp;
    }
    if (strcmp(p, dest) == 0)
      printf("%s\n", path);
    break;

  case T_DIR:
    strcpy(buf, path);
    p = buf+strlen(buf);
    if (*(p-1) != '/') *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
        continue;
    //   memmove(p, de.name, DIRSIZ);
      strcpy(p, de.name);
    //   p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf("find: cannot stat %s\n", buf);
        continue;
      }
    //   printf("%s\n", buf);
      find(buf, dest);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc < 2){
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
