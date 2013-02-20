#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

#define TAILLE_CHAINE_FICHIER 128

//déclaration variables globales

void LireChaine(char chaine[], int size) {
	fgets(chaine, size, stdin);
	chaine[strlen(chaine) - 1] = '\0';
}

int LireNombreEntier() {
    char chaine[64];
    fgets(chaine, 64, stdin);
  
    return atoi(chaine);
}

float LireNombreFlottant() {
    char chaine[64];
    fgets(chaine, 64, stdin);
  
    return atof(chaine);
}

void sysout2(char* chaine){
	printf("%s\n",chaine);
}

void sysout(char* chaine, int DEBUG){
	if(DEBUG) printf("%s\n",chaine);
}

void sysoutn(char* chaine, int DEBUG){
	if(DEBUG) printf("%s",chaine);
}

char* substring(const char* str, size_t begin, size_t len)
{
  if (str == 0 || strlen(str) == 0 || strlen(str) < begin || strlen(str) < (begin+len))
    return 0;

  return strndup(str + begin, len);
}

int chargementParametre(const char* nomDuFichier, int DEBUG, int SERVER){
	int state = 1;
	
	char * type;
	if(SERVER) type = "$server:";
	else type = "$client:";
	
	FILE* fp = fopen(nomDuFichier,"r");
	if(fp != NULL){
		//le fichier a été ouvert
		
		char ligne[128];
		int trouve = 1;
		
		while(fgets(ligne, TAILLE_CHAINE_FICHIER, fp) != NULL){
			
			if(ligne[0] != '#'){
				//pas un commentaire
				if(trouve == 0){
					//on est dans la partie désirée
					if(ligne[0] == '$'){
						//fin de la partie désirée
						trouve = 1;
						sysout("... terminé!", DEBUG);					
					}
					else{
						//traitement de la ligne
						sysout2(ligne);
					}
				}
				else{
					if(ligne[0] == '$' && strcmp(substring(ligne,0,8),type) == 0){
						if(DEBUG) printf("L'instruction %s a été detecté, début du chargement des paramètres associés...\n", type);
						trouve = 0;
					}
				}
			}
		}		
		fclose(fp);
	}
	else{
		state = 0;
	}
	return state;
}
