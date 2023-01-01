#include<stdio.h>
#include<stdlib.h>
#include<string.h>//include de+.

int main(int argc, char** argv){
  char str[]="Geeks-for-Geeks";
  char* token=strtok(str,"-");
  while(token!=NULL){
    printf("%s\n",token);
    token=strtok(NULL,"-");//$1=NULL!!!
  }
  //ça s'arrête à la fin.
  //<=>split_args via un découpeur().
  return 0;
}
