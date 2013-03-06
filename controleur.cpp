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
    cli->runClient();
}

void Controleur::runServer()
{
    //Methode console de la gestion du serveur
    Serveur * s = new Serveur(this);
    s->runServer();
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
