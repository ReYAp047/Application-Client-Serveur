#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h> 
#include <arpa/inet.h>  
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <pthread.h>




typedef struct Trad
{
        char mot[30];
	char eng[30];
}Trad;

typedef struct game
{
	char p1[30];
	char p2[30];
	int resultat;
}game;

	typedef struct inv
{
	char ch[30];
}inv;






long factoriel(int x){
int i;
long f=1;
for(i=1;i<=x;i++){
f=f*i;
}
return f;
}


void traduction(int socket){
FILE *f ;
char eng[30],fr[30];
int bb=1;
Trad ch;
while(bb==1){
	 f=fopen("dictionaire.txt","r");
         //recevoir le mot a traduire
         printf("En attend du mot ...\n");
         recv(socket, &ch, sizeof(Trad) ,0);
         printf("[i]Mot recu = : %s\n",ch.mot);

           if (strcmp(ch.mot,"null") == 0)
         {
                bb=0;
         }

         else{
                int trouv=0;
                while(fscanf(f,"%s %s",eng,fr)&& trouv==0)
                { if(strcmp(fr,ch.mot) == 0)
                  {
                            trouv=1;
				strcpy(ch.eng,eng); 
		   }else if(strcmp(eng,ch.mot) == 0)
		   {
		   	trouv=1;
			strcpy(ch.eng,fr); 
		   }
		   if (strcmp(eng,"nullnull")==0 && strcmp(fr,"nullnull")==0  ){
			   trouv=-1;
		}
		}
           if(trouv==1)
           {
                send(socket, &ch, sizeof(Trad), 0);
                }
           else{
		 strcpy(ch.eng,"Vérifier votre mot");
		 send(socket, &ch, sizeof(Trad), 0);
		}

          printf("[+]La traduction est evoyer au client %d\n",socket);
        	fclose(f); 
	}//
   
}//end of while bb==1
}//



int inverse(int socket){
	int k=1;
	inv mot;
	while(k==1){
	recv(socket, &mot, sizeof(inv) ,0);
	if (strcmp(mot.ch,"null")!=0){
	char rev[100];
    int t, i, j;
    j = 0;
    t = strlen(mot.ch);
    rev[t] = '\0'; //le dernier caractère doit toujours être égale à '\0'.
    for (i = t - 1; i >= 0; i--)
    {
      rev[j] = mot.ch[i];
      j++;
    }
    strcpy(mot.ch,rev);
    send(socket, &mot, sizeof(inv), 0);	
			}else{
				k=0;
				}	
		}
	}


void *function(void *arg)
{
  int socket = *(int*)arg;
  int x,rep,pr;
  long f;
  game jeux;
  int k=1;
  char reponse[30];
recv(socket, &rep, sizeof(int) ,0);
if(rep==1){
printf("[i]Le Client numéro %d  a choisir l'option : Factorielle .\n",socket);
int b=1;
while(b==1){

recv(socket, &x, sizeof(int) ,0);
  printf("[i]Entier recu = :   %d\n",x);
if(x==-1){
 b=0;
}else
{
f=factoriel(x);
  send(socket, &f, sizeof(long), 0);
   printf("[+]Réponce evoyer au client %d\n",socket);
}
}
}
else if(rep==2){
printf("[i]Le Client numéro %d  a choisir l'option : Jeux .\n",socket);
while(k==1){
//reception du nombre de la part du client
 recv(socket, &jeux, sizeof(game) ,0);
 if (jeux.resultat==-1){
    k=0;
 }else{
  printf("[i]Les propositions sont prises = : ");
    int r = rand()% 2;
	if (r == 1) {
		strcpy (reponse,"head");
	}
	else {
		strcpy (reponse,"tail");
	}
	
if (strcmp(jeux.p1,reponse)==0 || strcmp(jeux.p2,reponse)!=0 ){
  jeux.resultat= 1;
}else if((strcmp(jeux.p2,reponse)==0 || strcmp(jeux.p1,reponse)!=0 )){
  jeux.resultat =2;
  }else
  jeux.resultat = 0;

//l'envoi du resultat vers le client
  send(socket, &jeux, sizeof(game), 0);
   printf("[+]Réponce evoyer au client %d\n",socket);
}
}
}

else if(rep==3)
{

	printf("[i]Le Client numéro %d  a choisir l'option : Traduction .\n",socket);
traduction(socket);

}
else if(rep==4){
	printf("[i]Le Client numéro %d  a choisir l'option : Inverser .\n",socket);
	inverse(socket);
	
	}
printf("[-]Client %d est sortie \n",socket);
close(socket);
free(arg);
pthread_exit(NULL);
}

void main(){
	// AF_INET pour le ip v4
	//Sock_STEAM c'est le type du socket , type tcp 
	int socketServer = socket(AF_INET, SOCK_STREAM, 0) ;
	struct sockaddr_in addrServer;
	// inet_addr covertire au type demander
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);
	
	bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
	printf("[+]Bind \n");
	
	listen(socketServer, 5);
	printf("En ecoute...\n");
pthread_t threads[100];
int i=-1;
while(1){
	i++;
	struct sockaddr_in addrClient;
	socklen_t csize = sizeof(addrClient);
	int socketClient = accept(socketServer,(struct sockaddr *)&addrClient, &csize);

	printf("[+]Client %d accepter \n",socketClient);

	int *arg = malloc(sizeof(int));
	*arg = socketClient;
	pthread_create(&threads[i], NULL, function, arg);
}
for(int j = 0;i < i;j++)
{
	pthread_join(threads[j], NULL);
}
close(socketServer);
printf("close\n");
	}
