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
    int longueur;
    int port = controller->getPort();
    char bufMessage[BUFFER_MESSAGE_SIZE];

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
                QString chemin = QFileDialog::getOpenFileName(0,"fichier à envoyer",QString(),"fichier (*.*)");
                if(!chemin.isNull() && !chemin.isEmpty()){
                    //on a le fichier, il faut maintenant l'envoyer
                    envoieFile(chemin);
                    waitingForValidation();
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

void Client::envoieFile(QString path){

    QFile f(path);
    if(f.open(QIODevice::ReadOnly)){
        qDebug()<<"fichier ouvert";
        //int pos = 0;
        char data[BUFFER_FRAME];
        int sizeSend = f.size();
        int byteWritten;
        int byteRead = sizeSend;

        //envoi de la taille au serveur
        envoiTaille(sizeSend);

        while(!f.atEnd()){
            byteRead = f.read(data, BUFFER_FRAME);
            if(byteRead == 0) break;
            if(byteRead < 0){
                qDebug()<<"erreur lors de l'écriture du buffer";
                exit(1);
            }
            void* p = data;
            while(byteRead >0){
                qDebug()<<"envoie trame";
                byteWritten = write(socket_descriptor,p,byteRead);
                if(byteWritten <=0){
                    qDebug()<<"problème envoie trame";
                    exit(1);
                }
                byteRead -= byteWritten;
                //f.seek(f.pos() + byteWritten);
            }
        }
       /* while(pos<10000){
            qDebug()<<"pos :"+QString::number(pos);
            //on parcourt le fichier
            f.seek(pos);
            qDebug()<<"1";

            //byteRead = f.read(data,BUFFER_FRAME);
            QByteArray lal = f.read(BUFFER_FRAME);
            data = lal.data();
            qDebug()<<"2";
            if(lal.size() == BUFFER_FRAME){
                //la lecture est réussie
                pos += byteRead;

                //envoie de la trame
                envoiTrame(data);
            }
            qDebug()<<lal;
        }*/
    }
    else{
        qDebug()<<"impossible d'ouvrir le fichier";
    }
}

void Client::envoiTrame(char* data){

    qDebug()<<"envoiTrame";
    if(write(socket_descriptor,data,sizeof(data)) <0){
       qDebug()<<"impossible d'envoyer";
       exit(1);
    }

}

void Client::envoiTaille(int t){
    int tmp = htonl(t);
    if(write(socket_descriptor,&tmp,sizeof(tmp)) <0){
       perror("impossible d'envoyer");
       exit(1);
    }

}
void Client::waitingForValidation(){

    int longueur;
    char bufMessage[BUFFER_MESSAGE_SIZE];
    if((longueur = ::read(socket_descriptor, bufMessage, sizeof(bufMessage) -1)) > 0){
       perror("impossible d'envoyer");
       exit(1);
    }
    else{
        qDebug()<<bufMessage;
    }

}
