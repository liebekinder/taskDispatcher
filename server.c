/*
 * main.c
 * 
 * Copyright 2013 seb-linux <seb-linux@seblinux-VirtualBox>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#include "fonction.h"
#include "server.h"


#define TAILLE_MAX_NOM 256
#define NB_CLIENT 16			//nombre de connexion en attente
#define DEBUG 1					//mode non débogage par défaut
#define BUF_MESSAGE_SIZE 32		//taille du buffer de communication



typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

struct paramThread{
	int sock_;
	int DEBUG_;
	int NB_CLIENT_;
};

char* HOST = "127.0.0.1";
int* listeClient;					//liste des clients, sa taille est paramétrée dans la fonction principal (vaut NB_CLIENT)
pthread_t* listeThread[64];



int main(int argc, char **argv)
{
	//déclaration des variables

	int port = 5000;
	int socket_descriptor;
	//int longueur;
	sockaddr_in adresse_locale;
	hostent* ptr_hote;
	
	int nouv_socket_descriptor;
	int longueur_adresse_courante;
	sockaddr_in adresse_client_courant;
	//char* host = HOST;
	
	int ret = 0;
	pthread_t t_client;
	
	char machine[TAILLE_MAX_NOM+1];

   gethostname(machine,TAILLE_MAX_NOM); // renvoie dans machine le nom réseau de la machine (ip, nom hote,...)

   if((ptr_hote = gethostbyname(machine)) == NULL) //on récupère le pointeur vers hostent contenant entre autre le nom d'hote
   {
	   perror("erreur de récupération hote par son nom");
	   exit(1);
   }

   sysout("création serveur...",DEBUG);
   bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length); // on copie l'adresse de ptr_hote dans adresse_locale
   adresse_locale.sin_family = ptr_hote->h_addrtype;                                    // on copie le type d'adresse (AF_INET)
   adresse_locale.sin_addr.s_addr = INADDR_ANY;                                         // on assigne n'importe quelle ip locale à l'adresse: inet_addr("127.0.0.1") fonctionne aussi
   sysout("fin création serveur...",DEBUG);

   adresse_locale.sin_port = htons(port);                                               // affectation du port à l'adresse

   if (DEBUG) printf("numéro de port pour la co serveur: %d\n", ntohs(adresse_locale.sin_port));


   sysoutn("création socket...",DEBUG);
   //création de la socket

   if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
	   //création d'un socket en TCP/IP, mode connecté, par défaut
	   perror("impossible de creer la socket de co avec client");
	   exit(1);
   }

   if((bind(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
	   // liaison du socket avec la structure adresse (casté de sockaddr_in vers sockaddr). ajout taille.
	   perror("impossible de lier la socket avec client");
	   exit(1);
   }
   sysout("succes!",DEBUG);
   
	//----------------------------------------------------------------------//
   
   //allocation du tableau des clients, thread et aptitude
	listeClient = (int *) malloc(NB_CLIENT*sizeof(int));
	int i;
	for(i=0; i<NB_CLIENT; ++i){
		listeClient[i] = 0;
	}
	
	/*listeThread = (pthread_t *) malloc(NB_CLIENT*sizeof(pthread_t));
	for(i=0; i<NB_CLIENT; ++i){
		listeClient[i] = 0;
	}*/
   
   //creation d'un thread pour la gestion des clients (deco)
   
   //
   
   
   //----------------------------------------------------------------------//
   
	sysout("listen...",DEBUG);
	listen(socket_descriptor,NB_CLIENT);
	// écoute du port à travers le socket, NB_CLIENT connexion en attente max
	
	//attente des connexions
	for(;;){
		longueur_adresse_courante = sizeof(adresse_client_courant);
		if((nouv_socket_descriptor = accept(socket_descriptor,(sockaddr*)(&adresse_client_courant),(socklen_t *) &longueur_adresse_courante)) <0){
			//création d'un nouveau socket d'acceptation à partir du premier
			perror("pas acceptation");
			exit(1);
		}
		sysout("reception message",DEBUG);

		//thread
		sysout("création d'un thread...",DEBUG);
		ret = pthread_create(& t_client, NULL, connexionPrincipale,(void *) nouv_socket_descriptor);              //sauf cas particulier, le deuxième paramètre est a NULL, fonction en 3eme et ses paramètre (NULL si rien)
		if(ret){
			perror("erreur lors de la création du thread");
			exit(1);
		}
		else{
			sysout("thread démarré",DEBUG);
		}
	}
}
	

void * connexionPrincipale(void * sock2){
	
	char bufMessage[BUF_MESSAGE_SIZE];				//buffer de communication, pour les messages informatifs

	int sock = (int) sock2;
	int longueur;
    int ETATCO = slotDisponible(); //temporaire, vérifie si le serveur est plein ou pas
    
	if(ETATCO) bcopy("CO-OK", bufMessage, sizeof(bufMessage));
	else bcopy("CO-NO", bufMessage, sizeof(bufMessage));
    
    if(DEBUG) printf("message envoyé: %s\n",bufMessage);
	
	if(write(sock,bufMessage,sizeof(bufMessage)) <0){
	   perror("impossible d'envoyer");
	   exit(1);
	}
	
	if(ETATCO){
		
		int continu = 1;
		while(continu){
			if((longueur = read(sock, bufMessage, sizeof(bufMessage) - 1)) <= 0)
			//lecture du message du socket (taille à définir entre application)
				   continu = 0;
			if(DEBUG) printf("message lu: %s\n",bufMessage);
			
		}		
	}
	
	close(sock);
	//fermeture du socket
	sysout("fermeture du thread...",DEBUG);
	sysout("fermeture du socket...",DEBUG);
	void * a = 0;
	return a;
}

int slotDisponible(){
	int i;
	int sDispo = 0;
	for(i = 0; i < NB_CLIENT; ++i){
		if(listeClient[i] == 0) sDispo = 1;
	}
	return sDispo;
}


