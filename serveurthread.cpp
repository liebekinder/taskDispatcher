#include <QDebug>
#include <QFileDialog>

#include "serveurthread.h"
#include "Parametres.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/types.h>
#include <sys/socket.h>
#include <netdb.h>

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

    bool ETATCO = slotDisponible(); //temporaire, vérifie si le serveur est plein ou pas

    if(ETATCO) bcopy("CO-OK", bufMessage, sizeof(bufMessage));
    else bcopy("CO-NO", bufMessage, sizeof(bufMessage));

    if(write(new_sock,bufMessage,sizeof(bufMessage)) <0){
       perror("impossible d'envoyer");
       exit(1);
    }

    if(ETATCO){
        qDebug()<<"on attend la taille du fichier";

        int tailleFic;
        if((tailleFic = tailleFichier()) > 0){
            qDebug()<<"On attends le fichier de taille "+QString::number(tailleFic);
            recevoirFile("lol.txt",tailleFic);

            /*QString chemin = QFileDialog::getSaveFileName(0,"fichier à envoyer",QString(),"fichier (*.*)");
            if(!chemin.isNull() && !chemin.isEmpty()){
                //on a le fichier, il faut maintenant l'envoyer
                recevoirFile(chemin,tailleFic);
            }*/
        }
    }

    close(new_sock);
    //fermeture du socket
    qDebug()<<"fermeture du thread...";
    qDebug()<<"fermeture du socket...";

    emit finished();
}

void ServeurThread::recevoirFile(QString path, int size){

    char trame[BUFFER_FRAME];
    int longueur;
    int readByte = 0;

    QFile f(path);
    f.open(QIODevice::ReadWrite|QIODevice::Truncate);f.close();
    if(f.open(QIODevice::WriteOnly|QIODevice::Append)){

        while(readByte < size){
            longueur = read(new_sock, trame, BUFFER_FRAME);
            //if((longueur = read(new_sock, trame, BUFFER_TRAME)) <= 0){
            if(longueur > 0){
                //lecture du message du socket (taille à définir entre application)
                qDebug()<<"reception d'une trame";
                f.write(trame,longueur);
                readByte+=longueur;
            }
            else{
                if(longueur == -1) qDebug()<<"pas réussi à lire?";
                else{
                    qDebug()<<"trasnmisison's over!";
                    readByte=size;
                }
            }
        }
        //loop
    }
}

bool ServeurThread::slotDisponible(){
    return true;
}

int ServeurThread::tailleFichier(){
    int reponse = 0;
    if((read(new_sock, &reponse, sizeof(reponse))) > 0){
        //lecture du message du socket (taille à définir entre application)
        qDebug()<<"reception de la taille";
        //reponse = ntohl(trame);
    }
    else{
        qDebug()<<"problème :(";
    }
    return ntohl(reponse);
}
