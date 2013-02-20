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

    //fields
    static QList<int> * listeClient;


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

    static const int TAILLE_MAX_NOM = 256;
    static const int NB_CLIENT = 16;
    static const int BUFFER_MESSAGE_SIZE = 32;

    //methods
    static bool slotDisponible();
    static void * connexionPrincipale(void* sock2);


signals:
    
public slots:
    void client();
    void server();
    
};

#endif // CONTROLEUR_H
