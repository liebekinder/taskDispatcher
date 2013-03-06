#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class Controleur;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(Controleur * c, QObject *parent = 0);
    void runClient();

private:
    Controleur * controller;
    int socket_descriptor;

    //methodes

    void envoieFile(QString path);
    void envoiTrame(char* data);
    void envoiTaille(int t);
    void waitingForValidation();

signals:
    
public slots:
    
};

#endif // CLIENT_H
