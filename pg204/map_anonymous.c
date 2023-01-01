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
//on doit passer la valeur de PROT au pré-processeur.

//les inclusions habituelles
//mmap on aurait besoin du fichier, ondéfinit une macro au cas où.
//on aurait dû séparer les executions via els variables du pré-processeur
//mais cela va être fait après.

int main(int argc, char** argv){
  void* p=NULL;
  p=mmap(NULL,sizeof(int),PROT,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
  //le private dans l'exo2 aurait empêcher toute communication
  //2 processus sans mémoire partagée(n'accepte pas fd DONC.///).
  fprintf(stderr,"%p ",p);
  fprintf(stderr,"%d ",(*(int*)p));
  (*(int*)p)=42;
  fprintf(stderr,"%d ",*(int*)p);
  return 0;
}
  
/*
make map_anonymous CFLAGS+='-DPROT=PROT_NONE'  && mv map_anonymous map_none
./map_none
ici, PROT est mis à PROT_NONE.
1ere chose à faire: expliquer les PROT_NONE/WRITE/READ/EXEC
cela est en rapport avec les pages mémoires réservées.
soit le droit d'écrire, pas le droit d'y toucher, le droit de lire, ou bien
le droit d'executer.
avec -DPROT=PROT_NONE, on autorise à ne pas toucher aux pages ; en fait, on n'autorise rien.
=>ce qui va être affiché:
l'adresse p en void* puis segmentation fault car PROT n'est pas dans le read.
on l'a compris d'après le man de mmap:
rm map_none(execution d'avant précède une en fait pour la cmd.) && make map_anonymous CFLAGS+='-DPROT=PROT_READ'
./map_anonymous
=>p/0/segmentation fault (pas le droit d'ecrire)
le asm quand il va charger le ptr puis la valeur dans un registre et les droits
implicitement comprises en vérification aussi ça va accèder à une zone enmode nonce qui va impliquer segmentation fault(core dumped).
rm map_anonymous && make map_anonymous CFLAGS+='-DPROT=PROT_WRITE'
./map_anonymous
avec le write, on a le droit d'ecrire ce qui passe par une consultation donc aussi read soit -DPROTREAD implicitement faite.
PAR CONTRE, :
rm map_anonymous && make map_anonymous CFLAGS+='-DPROT=PROT_EXEC'
./map_anonymous va donner seulement les droits en exécution particulière
donc pas de read ni de write, cela donne: p segfault.
rm -f map_anonymous && make map_anonymous CFLAGS+='-DPROT="PROT_EXEC|PROT_WRITE"'
avec ceci, on obtient le même qu'avec le write.

<=>NONE,READ,WRITE,EXEC,"EXEC|WRITE" : 5 scénarios d'allocations après correction des erreurs.

c'est terminé avec l'exo3.
 */

  
