#include <QThread>

#include "controleur.h"
#include "fenetreprincipale.h"
#include "Parametres.h"
#include "serveur.h"
#include "client.h"



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
    //Method console de la gestion du client
    Client * cli = new Client(this);

    QThread * qth = new QThread;

    cli->moveToThread(qth);

    //gestion de la fonction à accrocher.
    connect(qth,SIGNAL(started()), cli, SLOT(process()));

    qth->start();
}

void Controleur::runServer()
{
    //Methode console de la gestion du serveur
    Serveur * s = new Serveur(this);

    QThread * qth = new QThread;

    s->moveToThread(qth);

    //gestion de la fonction à accrocher.
    connect(qth,SIGNAL(started()), s, SLOT(process()));
    qth->start();
}

void Controleur::client()
{
    port = fenetre->port->value();
    mode = CLIENT;

    //reglage ok, on affiche
    fenetre->setClientGUI();
    runClient();
}

void Controleur::server()
{
    port = fenetre->port->value();
    mode = SERVER;

    //reglage ok, on affiche
    fenetre->setServerGUI();
    runServer();
}

void Controleur::serverGetTelechargement()
{
    //maj graphique
}
