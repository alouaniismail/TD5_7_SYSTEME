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

void* data=NULL;


void myhandler_fct(int signum){
  printf("SIG NUM IS %d\n",signum);
}


int main(int argc, char** argv){

  sigset_t set;
  struct sigaction handler;
  handler.sa_handler=myhandler_fct;
  /*
  sigfillset(&handler.sa_mask);//ignorer tous les signaux(pas une bonne idée).
  */
  handler.sa_flags=0;
  sigaction(SIGUSR1,&handler,NULL);
  //installation du nouveau gestionnaire du signal SIGUSR1.
  sigemptyset(&set);

  int* int_data;
  //le ^@ est équivalent à \0 en char*<=>ascii_code[]_conversion().
  //continuité:(fd now)::
  
  int fd=open("text.txt",O_RDWR),pid;
  exit_if(fd==-1,"open");

  data=mmap(NULL,FILE_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  exit_if(data==MAP_FAILED,"map");

  close(fd);
  int_data=(int*)data;//il doit comme première action lire le pid de la zone
  //mémoire.(au début de la zone mémoire)
  pid=int_data[0];

  printf("le pid lu est:%d\n",pid);

  //2ième action: écrire dans le segment mémoire ce qu'il lit sur l'entrée
  //standard.

  int nbread=read(STDIN_FILENO,data,FILE_SIZE);
  exit_if(nbread==-1,"read");

  //FILE_SIZE c'est dans le write qu'on se méfit(rappel seulement).
  //envoie au processus avec pid le signal SIGUSR1((reserve)).

  char* str_data=(char*)data;
  str_data[nbread]='\0';//jusqu'à un caractère non pris en compte.
  //cf. TD1.

  
  //récupération du char* et mise dans une forme compatible.

  kill(pid,SIGUSR1);//derniere action, envoie du signal à pid.
  //synchronisation à action limitée.
  exit_if(munmap(data,FILE_SIZE)==-1,"error munmap occured. ");


  //on l'a dit(ou bien vient de le dire): la mémoire est limitée
  //pour le fichier. (mémoire glissante"").///
  //le ls -l pour appréhender la taille reste la même.
  return EXIT_SUCCESS;
}

//on doit lancer 2 fenêtres
//le handling ici n'est pas obligé
//./sig_receveur va afficher le pid
//./sig_expediteur va afficher le pid (le même)
//va faire: bbbbbbbbbbbbbb+entrée pour activer la lecture sur le flux entrant
//l'état dufichier complétement modifié avec kill(pid,SIGUSR1) ça va faire
//dans la fenêtre du ./sig_receveur
//SIG NUM IS 10(message prédéfini)
//+ce qu'il va être lu: le b(nbr_fois**) car après caractère bloquant.
//ls -l text.txt fait toujours 128 octets.
//voilà.
//2 executions vont donner 4 colonnes en text.txt avec tjrs 128 octets
//en taille(communication de processus implicite via des mémoires de partage
//via le mmaping d'un fichier dans chaque block(s) mém_virtuelle de processus.

//avec les ^@ au début bien sur à chaque fois.(resizing après le double*2+)(()).
