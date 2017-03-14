#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include"sapi.h"
#include"sparse.h"
#include"sconstants.h"
#include<stdio.h>
int main(int argc,char **argv){
  struct stat file_stat;
  if (stat(argv[1],&file_stat))
    error("file doesn't exist!\n");
  else{
    int fd=open(argv[1],O_RDONLY);
    char *buf=(char *)mmap(NULL,file_stat.st_size,PROT_READ,MAP_SHARED,fd,0);
    obj_t *exp=parse_source(buf);
    print(exp);
  }
}
