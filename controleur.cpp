#include "controleur.h"
#include "fenetreprincipale.h"

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

Controleur::Controleur(FenetrePrincipale * f, QObject *parent) :
    QObject(parent)
{
    fenetre = f;
}

int Controleur::getPort()
{
    return port;
}

void Controleur::runClient()
{
    //Method console de la gestion du serveur
}

void Controleur::runServer()
{
    //Methode console de la gestion du client

    int socket_descriptor;
    sockaddr_in adresse_locale; //Si erreur sur adresse_locale, vérifier si vraiment besoin du pointeur
    hostent* ptr_hote;
    int nouv_socket_descriptor;
    int longueur_adresse_courante;
    sockaddr_in  adresse_client_courant;   //Si erreur sur adresse_locale, vérifier si vraiment besoin du pointeur

    int ret;
    pthread_t t_client;

    QList<pthread_t> * listeThread = new QList<pthread_t>();
    Controleur::listeClient = new QList<int>();
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

    adresse_locale.sin_port = htons(port);                                               // affectation du port à l'adresse

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
        qDebug()<<"création d'un thread...";
        ret = pthread_create(& t_client, NULL, Controleur::connexionPrincipale,(void *) nouv_socket_descriptor);              //sauf cas particulier, le deuxième paramètre est a NULL, fonction en 3eme et ses paramètre (NULL si rien)
        if(ret){
            perror("erreur lors de la création du thread");
            exit(1);
        }
        else{
            qDebug()<<"thread démarré";
        }
    }

}

void Controleur::client()
{
    port = fenetre->port->value();
    mode = CLIENT;

    //reglage ok, on affiche
    fenetre->setClientGUI();
}

void Controleur::server()
{
    port = fenetre->port->value();
    mode = SERVER;

    //reglage ok, on affiche
    fenetre->setServerGUI();
}

void * Controleur::connexionPrincipale(void * sock2){

    char bufMessage[BUFFER_MESSAGE_SIZE];				//buffer de communication, pour les messages informatifs

    int sock = (int) sock2;
    int longueur;
    bool ETATCO = slotDisponible(); //temporaire, vérifie si le serveur est plein ou pas

    if(ETATCO) bcopy("CO-OK", bufMessage, sizeof(bufMessage));
    else bcopy("CO-NO", bufMessage, sizeof(bufMessage));

    qDebug()<<"message envoyé:";

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
            qDebug()<<"message lu"+ QString(bufMessage);

        }
    }

    close(sock);
    //fermeture du socket
    qDebug()<<"fermeture du thread...";
    qDebug()<<"fermeture du socket...";
    void * a = 0;
    return a;
}

bool Controleur::slotDisponible(){
    return (Controleur::listeClient->count() < NB_CLIENT);
}
