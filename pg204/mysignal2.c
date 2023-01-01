#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include "utils.h"

void myhandler_fct(int signum){
  printf("SIG NUM IS %d\n",signum);
}


int main(int argc, char* argv[]){
  sigset_t set;
  struct sigaction handler;
  struct sigaction old_handler;
  printf("My pid is %d \n",getpid());
  handler.sa_handler=myhandler_fct;
  sigfillset(&handler.sa_mask);
  handler.sa_flags=0;
  sigaction(SIGUSR1,&handler,&old_handler);
  sigemptyset(&set);
  while(1){
    sigsuspend(&set);//sauf le vide ecoute tous les signaux
    //<=>pause() avec le while ca redemarre encore.
  }
  sigaction(SIGUSR1,&old_handler,NULL);
  //restauration:>
  //Gestionnaire par défaut.
  return EXIT_SUCCESS;
  //lorsqu'on succede dans UN AUTRE TERMINAL avec sigstop puis sigcont
  //puis sigusr1 cela emerge depuis []+Arrete ./mysignal2
  //a un affichage de ce que cela fait avec des attentes
  //notamment de sigkill qui tue le processus mais tjrs ..il fautune entrée!
  //(clavier) car sigstop puis sigcont ouvrir une nouvelle tache pas de fond
  //puis sigusr1 qui traduit cela puis l'entrée pour compenser la perte avec
  //sigkill.(affichage abstrait avec l'entrée pas de []+ donc pas sigstop
  //crtl+Z mais sigkill(parmi les 63, c'est le 9ième.(Crtl+C)).

  //remarque le sigusr1 affiche tjrs ce qu'on voudrait pas '' et oui!
  //rq: voir la boucle.//
}
