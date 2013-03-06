#include <QDebug>
#include <QThread>
#include <QFileDialog>

#include "client.h"
#include "Parametres.h"
#include "controleur.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>

typedef struct sockaddr sockaddr;
typedef struct sockaddr_in sockaddr_in;
typedef struct hostent hostent;
typedef struct servent servent;


Client::Client(Controleur *c, QObject *parent) :
    QObject(parent)
{
    controller = c;
}

void Client::runClient(){

    sockaddr_in adresse_locale;
    hostent* ptr_hote;
    int socket_descriptor;
    int longueur;
    int port = controller->getPort();
    char bufMessage[32];

    if((ptr_hote = gethostbyname(HOST)) == NULL){
               perror("pas trouvé serveur");
               exit(1);
       }

    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length);
    adresse_locale.sin_family = AF_INET;

    adresse_locale.sin_port = htons(port);

    qDebug()<<"numéro de port pour la co serveur:" + QString::number(ntohs(adresse_locale.sin_port));

    if((socket_descriptor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
            perror("impossible de creer la socket de co avec client");
            exit(1);
    }

    // le :: devant connect le compilateur d'essayer de forcer l'utilisation de la methode connect
    //présente dans QObject et non pas celle de socket.h
    if((::connect(socket_descriptor, (sockaddr*)(&adresse_locale), (socklen_t)(sizeof(adresse_locale)))) < 0){
            perror("impossible de lier la socket avec client");
            exit(1);
    }

    qDebug()<<"...connexion ok!";

    //Attente de la réponse serveur
    if((longueur = ::read(socket_descriptor, bufMessage, sizeof(bufMessage) -1)) > 0){
            //
            if(strcmp(bufMessage,"CO-OK") == 0){
                         //connexion acceptée
                qDebug()<<"connexion acceptée";

                //le client chercher à envoyer un fichier
                QString chemin = QFileDialog::getOpenFileName(0,"fichier à envoyer",QString(),"fichier texte (*.txt)");
                if(!chemin.isNull() && !chemin.isEmpty()){
                    //on a le fichier, il faut maintenant l'envoyer

                }
            }
            else if(strcmp(bufMessage,"CO-NO") == 0){
                         //connexion refusée
                         qDebug()<<"connexion refusée";
            }
            else{
                    qDebug()<<"hmm.. comparaison foireuse";
            }
    }


    qDebug()<<"fin connexion";

    close(socket_descriptor);


    qDebug()<<"fin co";
}

void Client::envoieFile(QStrin path){
    QIODevice f(path);
    if(f.open(QFile::ReadOnly)){
        int pos = 0;
        qint64 byteRead = BUFFER_FRAME;
        char * data ;
        while(byteRead == BUFFER_FRAME){
            //on parcourt le fichier
            f.seek(pos);
            byteRead = f.readData(data,BUFFER_FRAME);
            if(f.readData(data,BUFFER_FRAME) != -1){
                //la lecture est réussie
                pos += byteRead;

                //envoie de la trame
                envoiTrame(data);
            }
        }
    }
    else{
        qDebug()<<"impossible d'ouvrir le fichier";
    }
}

void Client:envoiTrame(char* data){

}
