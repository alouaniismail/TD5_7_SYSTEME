#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include "utils.h"

void myhandler_fct(int signum){
  printf("SIG NUM IS %d\n",signum);
}


int main(int argc, char* argv[]){
  printf("My pid id %d\n",getpid());
  struct sigaction sa;
  struct sigaction sa2;
  sa.sa_handler=myhandler_fct;
  sigfillset(&sa.sa_mask);
  sa.sa_flags=0;
  sigaction(SIGUSR1,&sa,&sa2);
  kill(getpid(),SIGUSR1);
  pause();//d'ou vient l'attente.
  sigaction(SIGUSR1,&sa2,NULL);//restauration.(de cette façon 'redéfinition')
  kill(getpid(),SIGUSR1);//Signal #1 défini par l'usager.(voilà, c'est bon!)
  return 0;//même truc.  !///
}
//structure de base pour créer un gestionnaire capturant le signal SIGUSR1
//qui en approfondissement ne doit pas etre envoye a AUCUN PROCESSUS EN APPELS.
//Crtl-C car on envoie via kill ici (retrouve le même truc)
//pour terminaison du programme particuliere. 
