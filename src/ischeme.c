#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>
#include"stype.h"
#include"seval.h"
#include"sapi.h"
#include"sparse.h"
#include"sconstants.h"
#include"slib.h"
#include"sinit.h"
int main(int argc,char **argv){
  struct stat file_stat;
  if (stat(argv[1],&file_stat))
    error("file doesn't exist!\n");
  else{
    if (file_stat.st_size==0){
      print(&undef);
      return 0;
    }
    int fd=open(argv[1],O_RDONLY);
    char *buf=(char *)mmap(NULL,file_stat.st_size,PROT_READ,MAP_SHARED,fd,0);
    obj_t *exp=parse_source(buf);
    init();
    exp->ref++;
    print(eval(exp,&global_environment));
    close(fd);
  }
}
