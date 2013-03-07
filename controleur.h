#ifndef CONTROLEUR_H
#define CONTROLEUR_H

#include <QObject>
#include <QDebug>

class FenetrePrincipale;

class Controleur : public QObject
{
    Q_OBJECT
    Q_ENUMS(Mode)

public:
    explicit Controleur(FenetrePrincipale * f, QObject *parent = 0);
    enum Mode{SERVER,
         CLIENT};


    //get/set
    int getPort();

    //methods
    void runClient();
    void runServer();

private:
    //fields
    FenetrePrincipale * fenetre;
    int port;
    Mode mode;


    //methods
    //bool slotDisponible();
    //void * connexionPrincipale(void* sock2);


signals:
    
public slots:
    void client();
    void server();
    void serverGetTelechargement();
    
};

#endif // CONTROLEUR_H
