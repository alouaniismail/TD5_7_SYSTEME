#include <stdlib.h>
#include <stdio.h>

#include <fcntl.h>
#include <unistd.h>
#include "utils.h"
#include <sys/types.h>
#include <sys/ipc.h> //pour le ftok.(key_t type de retour)
#include <sys/shm.h>//le nouveau shm.h.
#include <sys/wait.h>
#include <semaphore.h>//utile ici, on expliquerait PQ.
#include <sys/stat.h>


#define BUFFER_SIZE 1024 //memory.h BUFSIZ aussi possible pour ne pas surcharger

int main(void){
  key_t key;
  int shmid;
  int* data; //attachement comme un tableau d'entiers.

  //on va calculer le nombre de processus via une boucle sur for
  //=>utiliser des sémaphores nommés paraît évident dans ce cas-ci.

  key=ftok("/etc/bashrc",2023);//-la taille paire habituelle.
  //le signal de terminaison operator const char*(void) const.
  
  int nbsons=0,master=1;//i et index correspondront aux variables de boucle.(*)

  
  sem_t* sem2;//le sémaphore initial.
  sem2=sem_open("/pidlist",O_CREAT,0666,1);
  sem_t sem=*sem2;
  //on crée un sémaphore avec la valeur initiale de 1 via un mode de type mode_t
  //0666(accessiblitée partout via plusieurs modes de manipulations)
  //inclut:
  //O_CREAT qui inclut la création implicitement.(ou le partage si existants)

  shmid=shmget(key,BUFFER_SIZE,IPC_CREAT|0644);
  data=shmat(shmid,NULL,0);
  //on a récupéré l'id. de mémoire partagée: shmid.
  //puis via le shmid on fait un shmat avec ce dernier NULL,0 pour $2,$3
  //pour récupérer le début void* de segment de mémoire partagée.
  //(entre différents processus/threads).

  int i;//(*)
  
  for(i=0;i<5;++i){//4 fois.
    switch(fork()){
    case 0:
      nbsons=0;
      master=0;
    default:
      nbsons++;
    }
  }

  int index;//va contenir la valeur de la variable courante de tous
  //les processus participant.
  //le sémaphore a la valeur 1 au début
  //pour éviter que plusieurs processus ne s'effectuent en même temps,
  //on va faire un wait=>sem_wait.
  sem_wait(&sem);//le sémaphore est bloquant.
  data[0]++;//à 1 normalement si à 0 tt au début.(en int*)
  index=data[0];
  sem_post(&sem);//on débloque.(même si nbsons revient à zéro, on comptabilise
  //bien les processus)

  data[index]=getpid();//si on suppose la superposition de la valeur
  //avec tous les processus .

  for(i=0;i<nbsons;++i){
    exit_if(wait(NULL)==-1,"wait");
  }//chaque parent sous-jacent attend la terminaison du fils
  //n'a aucun rapport avec ce qu'on fait actuellement.(avant je désigne)

  if(master){
    //gestion erreur par instruction séquentielle².
    fprintf(stderr,"Il y avait %d processus",data[0]);//cela peut croisement varier ; c'est ce qu'on remarque principalement.

    
    for(i=0;i<data[0];++i){
      fprintf(stderr,"%d",data[i+1]);
    }
  }//en négligeant le tout premier de source qui a fait cela
  //le dernier en l'inverse des choses.(())

  exit_if(shmctl(shmid,IPC_RMID,NULL)==-1,"shmctl");//déstruction de ce qui a été fait pour.
  
  exit_if(shmdt(data)==-1,"shmdt");//détachement.(accès=>segfault).

  sem_close(&sem);//au lieu de sem_destroy pour les sémaphores anonymes
  return EXIT_SUCCESS;
}


//se rappeler: une boucle sur fork() avec 4 tours
//=>2**4 *2 (<(++)((value));++) processus en tout<=> 32.

//on a 31,32 ; cela oscille.
//même si la solution d'épargner ceci et d'en ajouter un sémaphore
//on reste sur un fichier inexistant(cf. /etc/bashrc c'est /etc/bash.bashrc
//<=>~/.bashrc en caché pour le chargement).


//FIN DU CINQUIEME AVANT DERNIER TD.

  
  
