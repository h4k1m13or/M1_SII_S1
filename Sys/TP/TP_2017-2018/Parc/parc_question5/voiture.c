#include <sys/shm.h>
#include "semaphores2.h"

//structure de la mémoire partagée
typedef struct data{
int nbEmbarques;
int nbDebarques;
}shdata;

//fonction chargement
void chargement(int n)
{
	printf("Nouveau chargement, tournée numéro %d \n",n);
}

//fonction rouler
void rouler()
{
	printf(" tshout tshout !!! le train roule \n");
	printf(" __________ \n");
	printf("|   train  \\_ \n");
	printf(" o-o-o-o-o-o-| \n");
	sleep(2);
	printf(" __________    \t       \t   __________\n");
	printf("|   train  \\_ \t ----> \t  |   train  \\_ \n");
	printf(" o-o-o-o-o-o-| \t  \t   o-o-o-o-o-o-| \n");
	sleep(2);
	printf(" __________    \t        \t   __________  \t          \t   __________\n");
	printf("|   train  \\_ \t  ----> \t  |   train  \\_  \t ----> \t  |   train  \\_ \n");
	printf(" o-o-o-o-o-o-| \t        \t   o-o-o-o-o-o-| \t    \t   o-o-o-o-o-o-| \n");
	sleep(2);
}

//fonction Decharger
void Decharger()
{
	printf("\n Train prêt pour le déchargement \n");
}


/*****************************PROGRAMME PRINCIPALE****************************/


int main(void)
{
int n = 1; int p = 4;
int i,j;
int semEmbarquement = 0; int semDebarquement = 1;
int semTousDehors = 2; int semTousAbord = 3;

//mettre dans ce fichier la création et initialisation de tout les sémaphores nécessaires.
//creation d'une clé
key_t key = ftok("/home/izan/Desktop/Parc",'e');

//création de l"espace mémoire partagé
int shmid = shmget(key,sizeof(shdata), IPC_CREAT | IPC_EXCL | 0666);
if(shmid==-1){
//la zone existe deja !
shmid=shmget(key,sizeof(shdata),0); //recuperer son id
}

//attacher le processus a la zone de mémoire partagée
shdata *sd= NULL;
sd = shmat(shmid,sd,0);

//initialiser les variables partagées
sd->nbEmbarques = 0;
sd->nbDebarques = 0;

//création des sémaphores
//groupe de 4 semaphores
int semid = semcreate(key,4);

//initialiser les sémaphores
seminit(semid , semEmbarquement , 0);
seminit(semid , semDebarquement , 0);
seminit(semid , semTousDehors , 0);
seminit(semid , semTousAbord , 0);


/****************************************************************************************/


while (1){

chargement(n); n++;

for( i = 1 ; i <=p ; i++){V(semid, semEmbarquement );

}

					//ici on remplace cette ancienne implementation
					//P(semid, semTousAbord); puis V(semid, semDebarquement); 
					//par celle ci :
					/*********************************************************************/
					/**/      Synchro(semid, semTousAbord,n , p,semDebarquement);      /**/
					/*********************************************************************/


rouler(); 
Decharger(); 

P(semid, semTousDehors); 
}

//semdelete(semid);
return 0;
}
