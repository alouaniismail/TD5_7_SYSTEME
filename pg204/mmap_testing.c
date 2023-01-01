#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include "utils.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_SIZE 128
void* data;

int main(int argc, char *argv[])
{
    int fd;

    fd = open("text.txt",O_RDWR);
    exit_if(fd == -1 , "open");
    
    data = mmap(NULL, FILE_SIZE, PROT_READ | PROT_WRITE , MAP_SHARED, fd , 0);
    exit_if( data == MAP_FAILED,"map");
    
    close(fd);
    char* start=(char*)data;
    int i;
    for(i=0;i<FILE_SIZE;i++){
      printf("%c",*(start++));
    }

    return 0;
    //familiarisation avec.(seulement!)
}
