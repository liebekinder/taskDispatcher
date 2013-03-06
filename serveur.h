#ifndef SERVEUR_H
#define SERVEUR_H

#include <QObject>
class Controleur;

class Serveur : public QObject
{
    Q_OBJECT
public:

    explicit Serveur(Controleur * c, QObject *parent = 0);
    void runServer();
    void connexionPrincipale(int sock);

private:

    //fields
    Controleur * controller;
    static QList<int> * listeClient;

    //methods

signals:

    void finished();
    void error(QString err);

public slots:
    void threadError(QString err);
    void manageIncomingCo();
};

#endif // SERVEUR_H
