#include <QDebug>

#include "serveurthread.h"
#include "Parametres.h"


ServeurThread::ServeurThread(Serveur *s, int sock, QObject *parent) :
    QObject(parent)
{
    server = s;
    new_sock = sock;
}


void ServeurThread::process()
{
    qDebug()<<"thread? :)";

    char bufMessage[BUFFER_MESSAGE_SIZE];				//buffer de communication, pour les messages informatifs

    int longueur;
    bool ETATCO = slotDisponible(); //temporaire, vérifie si le serveur est plein ou pas

    if(ETATCO) bcopy("CO-OK", bufMessage, sizeof(bufMessage));
    else bcopy("CO-NO", bufMessage, sizeof(bufMessage));

    qDebug()<<"message envoyé:";

    if(write(new_sock,bufMessage,sizeof(bufMessage)) <0){
       perror("impossible d'envoyer");
       exit(1);
    }

    if(ETATCO){

        int continu = 1;
        while(continu){
            if((longueur = read(new_sock, bufMessage, sizeof(bufMessage) - 1)) <= 0)
            //lecture du message du socket (taille à définir entre application)
                   continu = 0;
            qDebug()<<"message lu"+ QString(bufMessage);

        }
    }

    //traitement du fichier



    //fin traitement du fichier


    close(new_sock);
    //fermeture du socket
    qDebug()<<"fermeture du thread...";
    qDebug()<<"fermeture du socket...";

    emit finished();
}


bool ServeurThread::slotDisponible(){
    return true;
}
