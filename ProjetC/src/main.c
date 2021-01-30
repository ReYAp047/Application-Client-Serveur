#include <gtk/gtk.h>
#include <glib.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h>
#include <netdb.h> 
#include <arpa/inet.h>  
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
//AF_INET pour le ip v4
	//Sock_STEAM c'est le type du socket , type tcp 
	int socketClient ;
	
	struct sockaddr_in addrClient;


	typedef struct Trad
{
	char mot[30];
	char eng[30];
}Trad;

typedef struct game
{
	char p1[30];
	char p2[30];
	int  resultat;
}game;

	typedef struct inv
{
	char ch[30];
}inv;

    GtkBuilder *builder; 
    GtkWidget *window;
    GtkWidget *g_labelHT;
    GtkWidget *g_inputFact;
    GtkWidget *g_outputFact;
    GtkWidget *g_inputFrAng;
    GtkWidget *g_outputFrAng;
    GtkWidget *g_inputAngFr;
    GtkWidget *g_outputAngFr;
    GtkWidget *g_player1;
    GtkWidget *g_player2;
    GtkWidget *g_rp1;
    GtkWidget *g_rp2;
    
int main(int argc, char *argv[])
{
	socketClient= socket(AF_INET, SOCK_STREAM, 0) ;
	// inet_addr covertire au type demander
	addrClient.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(30000);
	printf("Vous etes sur l'adresse : 127.0.0.1 \n"); 
	connect(socketClient, (const struct sockaddr *)&addrClient, sizeof(addrClient));
	printf("[+]Connecté au serveur avec le port : 30000 \n");
	
	
    GtkBuilder      *builder; 
    GtkWidget       *window;

    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/interface.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
    gtk_builder_connect_signals(builder, NULL);
    
    g_object_unref(builder);

    gtk_widget_show(window);                
    gtk_main();

    return 0;
}
/*********************Traduction de Francaise vers Anglais*******************************/
void TraductionFrAng (){
	 int x=3;
	send(socketClient, &x, sizeof(int), 0);
	
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/frang.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main4"));
    gtk_builder_connect_signals(builder, NULL);
    
    g_inputFrAng = GTK_WIDGET(gtk_builder_get_object(builder, "inputFrAng"));
    g_outputFrAng = GTK_WIDGET(gtk_builder_get_object(builder, "outputFrAng"));
    g_object_unref(builder);
    gtk_widget_show(window);   
	
	}


void translateFrAng(){
	
	char ch[30]; 
	
	 //Le saisie du mot a traduire
	Trad dic;
    strcpy (ch,gtk_entry_get_text(GTK_ENTRY(g_inputFrAng)));
    strcpy (dic.mot,ch);
			//l'envoi du nombre vers le serveur
    send(socketClient, &dic, sizeof(Trad), 0);
		
	recv(socketClient, &dic, sizeof(Trad) ,0);
	strcpy(ch,dic.eng);
	gtk_label_set_text(GTK_LABEL(g_outputFrAng), ch);
	

}

/*********************Traduction de Anglais vers Francais*******************************/
void Inverse(){
	int x=4;
	send(socketClient, &x, sizeof(int), 0);
	
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/angfr.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main5"));
    gtk_builder_connect_signals(builder, NULL);
    
    g_inputAngFr = GTK_WIDGET(gtk_builder_get_object(builder, "inputAngFr"));
    g_outputAngFr = GTK_WIDGET(gtk_builder_get_object(builder, "outputAngFr"));
    g_object_unref(builder);
    gtk_widget_show(window);   
	
	}


void translateAngFr(){
	
	inv mot; 
	
	strcpy (mot.ch,gtk_entry_get_text(GTK_ENTRY(g_inputAngFr)));
	send(socketClient, &mot, sizeof(inv), 0);
	
	recv(socketClient, &mot, sizeof(inv) ,0);
	
	gtk_label_set_text(GTK_LABEL(g_outputAngFr), mot.ch);
	
}




/*********************Factoriel*******************************/

void pageFact(){
	  int x=1;
	send(socketClient, &x, sizeof(int), 0);
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/factoriel.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main2"));
    gtk_builder_connect_signals(builder, NULL);
    
    g_inputFact = GTK_WIDGET(gtk_builder_get_object(builder, "inputFact"));
    g_outputFact = GTK_WIDGET(gtk_builder_get_object(builder, "outputFact"));
    g_object_unref(builder);
    gtk_widget_show(window); 
}


void Calcule (){
	gint num = atoi((char *)gtk_entry_get_text(GTK_ENTRY(g_inputFact)));
		
    gchar buffer[32];
    glong fact=1;
		
	//l'envoi du nombre vers le serveur
    send(socketClient, &num, sizeof(int), 0);
       	        
    //reception de la resultat de la part du serveur
    recv(socketClient, &fact, sizeof(long) ,0);

    snprintf(buffer, sizeof(buffer), "Result: %ld", fact);

    gtk_label_set_text(GTK_LABEL(g_outputFact), buffer);
}






/*********************Heads Or Tails*******************************/

void game1()
{
	int x=2;
	send(socketClient, &x, sizeof(int), 0);
	builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "glade/game1.glade", NULL);

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main3"));
    gtk_builder_connect_signals(builder, NULL);
    g_player1 = GTK_WIDGET(gtk_builder_get_object(builder, "player1"));
    g_player2 = GTK_WIDGET(gtk_builder_get_object(builder, "player2"));
    g_labelHT = GTK_WIDGET(gtk_builder_get_object(builder, "labelHT"));
    g_rp1 = GTK_WIDGET(gtk_builder_get_object(builder, "rp1"));
    g_rp2 = GTK_WIDGET(gtk_builder_get_object(builder, "rp2"));
    g_object_unref(builder);
    gtk_widget_show(window);  
}
int player1;
int player2;
void gameHT(){
	game jeux;
	
	strcpy (jeux.p1,gtk_entry_get_text(GTK_ENTRY(g_player1)));
	strcpy (jeux.p2,gtk_entry_get_text(GTK_ENTRY(g_player2)));

	send(socketClient, &jeux, sizeof(game), 0);

    recv(socketClient, &jeux, sizeof(game) ,0);
    if (jeux.resultat == 1){
       player1=player1+1;
       gtk_label_set_text(GTK_LABEL(g_labelHT), jeux.p1 );
    }else if (jeux.resultat == 2){
	   player2=player2+1;
	   gtk_label_set_text(GTK_LABEL(g_labelHT), jeux.p2 );
  	}
    
    gchar p1[32];
    snprintf(p1, sizeof(p1), "Result: %d", player1);
    gchar p2[32];
    snprintf(p2, sizeof(p2), "Result: %d", player2);
	gtk_label_set_text(GTK_LABEL(g_rp1), p1);
	gtk_label_set_text(GTK_LABEL(g_rp2), p2);
	}
	
	
/*********************window close*******************************/	
void on_interface_destroy()
{
	
    gtk_main_quit();
}

void fermerFA()
{
	Trad dic;
		strcpy (dic.mot,"null");
	       	send(socketClient, &dic, sizeof(Trad), 0);
		
	
    gtk_main_quit();
}

void fermerFact()
{
	int x=-1;
    send(socketClient, &x, sizeof(int), 0);
		
    gtk_main_quit();
}

void fermerGame()
{
	game jeux;
	jeux.resultat=-1;
	send(socketClient, &jeux, sizeof(game), 0);
    gtk_main_quit();
}

void fermerInv()
{
	inv mot;
	strcpy(mot.ch,"null");
    send(socketClient, &mot, sizeof(inv), 0);
		
    gtk_main_quit();
}




