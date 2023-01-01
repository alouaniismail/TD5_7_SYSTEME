#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <signal.h>
#include "utils.h"
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_SIZE 128 //pour le "text.txt".

//pas de synchronisation immédiate, report à l'appel de munmap.
//préparation du retour de mmap avec void* d'un ptr à NULL.

void* data=NULL; //en global bien sûr.
//ce fichier doit faire les actions suivantes:
//mmaper le fichier (mémoire limitee) sur la mémoire virtuelle de l'exécutable.


//écrire le PID au début de la zone mémoire partagée.
//se met en attente avec sigsupsend sur le signal SIGUSR1
//(reprend l'exo. précédent.)

//signal reçu=>afficher le contenu du segment mémoire partagée en plus d'un message prédéfini.

void myhandler_fct(int signum){
  char* str_data=(char*)data;
  printf("SIG NUM IS %d\n",signum);
  if(data!=NULL){
    printf("j'ai reçu le msg:%s\n",str_data);
  }
}

int main(int argc, char** argv){
  printf("My pid for receiving signal and handling it:%d\n",getpid());
  sigset_t set;
  struct sigaction handler;
  int fd;
  int* int_data;
  fd=open("text.txt",O_RDWR);
  exit_if(fd==-1,"open");
  data=mmap(NULL,FILE_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  exit_if(data==MAP_FAILED,"mmap");//inclusion de <sys/mman.h>
  //puissant appel système.
  close(fd);//pas besoin de lui à cet instant d'instructions.
  int_data=(int*)data;




  /*int_data[0]=1;
   */

  //car sur 4 octets seulement
  //on veut ecrire un octet en entier ou bien un multiple soit 8 octets
  //on procède de cette façon.(<=>écrire une adresse)
  //on peut écrire getpid() tout seul(4 cases en réservations)
  //si juste pour s'assurer du nombre.
  //on obtient 4 cases dans ce cas-ci ce qui cohérent
  //il y a une traduction d'une table de 4 bites en un char
  //c'est pour cela on obtient l'affichage via: cat text.txt.

  
  int_data[0]=getpid();//ecriture du pid au début de la zone mémoire.
  //  printf("My pid is %d\n",getpid());
  //pas besoin de vérifications inutiles.
  handler.sa_handler=myhandler_fct;
  sigemptyset(&set);
  handler.sa_flags=0;
  sigfillset(&set);
  sigdelset(&set,SIGUSR1);
  //le masque on lui retire SIGUSR1, il va ignorer tout sauf SIGUSR1.
  //voilà ce que cela signifie .
  sigaction(SIGUSR1,&handler,NULL);
  sigsuspend(&set);//écouter le signal SIGUSR1 et ignorer tous les autres.
  //si on envoie le signal: on affiche un message prédéfini
  //plus le message de (char*)data en entier après avoir écrit
  //le PID du processus courant en début.
  //on fait le munmap pour faire une synchro .
  //direct:
  exit_if(munmap(data,FILE_SIZE)==-1,"error munmap occured.");
  //il attend un signal USR1
  //envoyé via ./sig_expediteur en troisième action
  //puis sortie normal bien sûr.
  //les char* l'emporte.

  //rQ: communication impliquant des signaux
  //plus que necessaire: overflow(segmentation fault), un peu (décallage) :
  //on parle de fenêtre partagée glissante.
  return EXIT_SUCCESS;
}
  


/*
si on ecrit plus ca s'execute et une nouvelle $s'affiche avec le reste
réclamant une commande qui est destiné à être introuvable dans le contexte
courant.
=>remplacement total du contenu du fichier(utile pour redémarrer à 0).
VOILA.
pas de comportements bizarres(même la tâche est terminée) et l'affichage 
de la part de celui-là(./sig_receveur 2ième action(*))
est bonne(affichage du cat text.txt après.)///
*/
