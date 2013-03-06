#include <QDebug>
#include <QThread>

#include "serveur.h"
#include "Parametres.h"
#include "controleur.h"
#include "serveurthread.h"


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


Serveur::Serveur(Controleur *c, QObject *parent) :
    QObject(parent)
{
    controller = c;
}

void Serveur::runServer()
{
    //Methode console de la gestion du client

    int socket_descriptor;
    sockaddr_in adresse_locale; //Si erreur sur adresse_locale, vérifier si vraiment besoin du pointeur
    hostent* ptr_hote;
    int nouv_socket_descriptor;
    int longueur_adresse_courante;
    sockaddr_in  adresse_client_courant;   //Si erreur sur adresse_locale, vérifier si vraiment besoin du pointeur

    char machine[256];

    gethostname(machine,TAILLE_MAX_NOM); // renvoie dans machine le nom réseau de la machine (ip, nom hote,...)

    if((ptr_hote = gethostbyname(machine)) == NULL) //on récupère le pointeur vers hostent contenant entre autre le nom d'hote
    {
        perror("erreur de récupération hote par son nom");
        exit(1);
    }

    qDebug()<<"création serveur...";
    bcopy((char*)ptr_hote->h_addr, (char*)&adresse_locale.sin_addr, ptr_hote->h_length); // on copie l'adresse de ptr_hote dans adresse_locale
    adresse_locale.sin_family = ptr_hote->h_addrtype;                                    // on copie le type d'adresse (AF_INET)
    adresse_locale.sin_addr.s_addr = INADDR_ANY;                                         // on assigne n'importe quelle ip locale à l'adresse: inet_addr("127.0.0.1") fonctionne aussi
    qDebug()<<"fin création serveur...";

    adresse_locale.sin_port = htons(controller->getPort());                                               // affectation du port à l'adresse

    qDebug()<<"numéro de port pour la co serveur:" + QString::number(ntohs(adresse_locale.sin_port));

    qDebug()<<"creation du socket...";

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

    qDebug()<<"creation avec succès du socket...";

    //----------------------------------------------------------------------//



   //----------------------------------------------------------------------//


    qDebug()<<"listen...";

    listen(socket_descriptor,NB_CLIENT);
    // écoute du port à travers le socket, NB_CLIENT connexion en attente max

    QList<QObject*> * listeThreadClient = new QList<QObject*>();
    int cptList = 0;

    for(;;)
    {
        longueur_adresse_courante = sizeof(adresse_client_courant);
        if((nouv_socket_descriptor = accept(socket_descriptor,(sockaddr*)(&adresse_client_courant),(socklen_t *) &longueur_adresse_courante)) <0){
            //création d'un nouveau socket d'acceptation à partir du premier
            perror("pas acceptation");
            exit(1);
        }
        qDebug()<<"reception message";

        //thread
        qDebug()<<"création d'un thread..." + QString::number(cptList);

        listeThreadClient->push_back((QObject*)(new QThread));
        listeThreadClient->push_back((QObject*)(new ServeurThread(this, nouv_socket_descriptor)));

        //MODIFICATIONS LOURDES

        ((ServeurThread*)(listeThreadClient->at(cptList+1)))->moveToThread((QThread*)(listeThreadClient->at(cptList)));
        connect((ServeurThread*)(listeThreadClient->at(cptList+1)),SIGNAL(error(QString)), this, SLOT(threadError(QString)));
        connect((QThread*)(listeThreadClient->at(cptList)),SIGNAL(started()), (ServeurThread*)(listeThreadClient->at(cptList+1)), SLOT(process()));
        connect((ServeurThread*)(listeThreadClient->at(cptList+1)),SIGNAL(finished()), (QThread*)(listeThreadClient->at(cptList)), SLOT(quit()));
        connect((ServeurThread*)(listeThreadClient->at(cptList+1)),SIGNAL(finished()), (ServeurThread*)(listeThreadClient->at(cptList+1)), SLOT(deleteLater()));
        connect((QThread*)(listeThreadClient->at(cptList)),SIGNAL(finished()), (QThread*)(listeThreadClient->at(cptList)), SLOT(deleteLater()));
        ((QThread*)(listeThreadClient->at(cptList)))->start();
        cptList += 2;


        //FIN GROSSES MODIFICATIONS
    }

}


void Serveur::connexionPrincipale(int sock){
}


void Serveur::threadError(QString err){
    qDebug()<<"erreur :" + err;
}

void Serveur::manageIncomingCo(){
    //all allocation must be here
    qDebug()<<"thread? :)";
    emit finished();
}
