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
#include "client.h"
#include "server.h"


#define TAILLE_MAX_NOM 256
#define NB_CLIENT 16			//nombre de connexion en attente
#define DEBUG 1					//mode non débogage par défaut


char* HOST = "127.0.0.1";
char bufMessage[32];

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;

struct paramThread{
	int sock_;
	int DEBUG_;
	int NB_CLIENT_;
};


int main(int argc, char **argv)
{
sockaddr_in adresse_locale;
hostent* ptr_hote;
int socket_descriptor;
int longueur;
int port = 5000;
char* host = HOST;


if((ptr_hote = gethostbyname(host)) == NULL){
		   perror("pas trouvé serveur");
		   exit(1);
	   }
	   
	   bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
	   adresse_locale.sin_family = AF_INET;

	   adresse_locale.sin_port = htons(port);

	   if(DEBUG) printf("numéro de port pour la co serveur: %d\n", ntohs(adresse_locale.sin_port));

	   if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		   perror("impossible de creer la socket de co avec client");
		   exit(1);
	   }

	   if((connect(socket_descriptor, (sockaddr*)(&adresse_locale), sizeof(adresse_locale))) < 0){
		   perror("impossible de lier la socket avec client");
		   exit(1);
	   }

	   sysout("...connexion ok!",DEBUG);   
	   
	   //Attente de la réponse serveur
	   if((longueur = read(socket_descriptor, bufMessage, sizeof(bufMessage) -1)) > 0){
		   //
		   if(strcmp(bufMessage,"CO-OK") == 0){
				//connexion acceptée
				sysout("connexion acceptée",DEBUG);
		   }
		   else if(strcmp(bufMessage,"CO-NO") == 0){
				//connexion refusée
				sysout("connexion refusée",DEBUG);
		   }
		   else{
			   sysout("hmm.. comparaison foireuse",DEBUG);
		   }
	   }
	   
	   sysout("fin connexion",DEBUG);

	   close(socket_descriptor);
	   

	   sysout("fin co",DEBUG);
	return 0;
}



