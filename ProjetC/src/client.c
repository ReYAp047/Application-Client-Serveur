#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h> 
#include <arpa/inet.h>  
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

typedef struct Trad
{
	char mot[30];
	char eng[30];
}Trad;

void main(){
	// AF_INET pour le ip v4
	//Sock_STEAM c'est le type du socket , type tcp 
	int socketClient = socket(AF_INET, SOCK_STREAM, 0) ;
	struct sockaddr_in addrClient;
	// inet_addr covertire au type demander
	addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(30000);
	printf("Vous etes sur l'adresse : 127.0.0.1 \n"); 
	connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
	printf("[+]Connecté au serveur avec le port : 3000 \n");
    	int rep;
	printf("Si vous voulez le factorielle\t Saisier : 1 \n");
	printf("Si vous voulez verfifier si un nombre est premier\t Saisier : 2\n"); 
	printf("Si vous voulez traduire un mot est FRANCAIS vers ANGLAIS\t Saisir : 3\n");
	printf("Si vous voulez traduire un mot est ANGLAIS vers FRANCAIS\t Saisir : 4\n");
	scanf("%d",&rep);
	send(socketClient, &rep, sizeof(int), 0);
	int x,pr;
	long y;
		if(rep==1){
		 printf("[i]Vous avez choisir : Factorielle\n");
		//Le saisie du nombre a calucler son factorielle
		printf("Saisir un nombre : \t ");
        	scanf("%d",&x);
		//l'envoi du nombre vers le serveur
       	        send(socketClient, &x, sizeof(int), 0);
		//reception de la resultat de la part du serveur
        	recv(socketClient, &y, sizeof(long) ,0);
        	printf("Le factoriel du nombre %d  =  %li \n",x,y);
		}

		else if(rep == 2){
		printf("[i]Vous avez choisir : Premier\n");
		 //Le saisie du nombre pour verifier sa nature
                printf("Saisir un nombre : \t ");
                scanf("%d",&x);
                //l'envoi du nombre vers le serveur
                send(socketClient, &x, sizeof(int), 0);
                //reception de la resultat de la part du serveur
                recv(socketClient, &pr, sizeof(int) ,0);
		if(pr==1)
                printf("Le nombre %d est : Premier\n",x);
		else
		printf("Le nombre %d est : N'est pas Premier \n",x);
		}

		else if(rep==3 || rep==4){
		 printf("[i]Vous avez choisir : Traduction\n");
		 //Le saisie du mot a traduire
		Trad ch;
		int bb=1;
		while(bb==1){
                	printf("Saisir le mot : \t ");
                	scanf("%s",ch.mot);
			//l'envoi du nombre vers le serveur
                	send(socketClient, &ch, sizeof(Trad), 0);
		if (strcmp(ch.mot,"null") == 0)
		{
			bb=0;
		}

		else{
			 //reception de la resultat de la part du serveur
			recv(socketClient, &ch, sizeof(Trad) ,0);
			printf("La traduction du mot est :  %s\n",ch.eng);
		}
		}
		}
close(socketClient);
printf("[-]Déconnexion du serveur \n");
	
	}
