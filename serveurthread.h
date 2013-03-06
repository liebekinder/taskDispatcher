#ifndef SERVEURTHREAD_H
#define SERVEURTHREAD_H

#include <QObject>

class Serveur;

class ServeurThread : public QObject
{
    Q_OBJECT
public:
    explicit ServeurThread(Serveur * s, int sock, QObject *parent = 0);

    //methods

    bool slotDisponible();
    
private:
    int new_sock;
    Serveur * server;

    //methodes
    void recevoirFile(QString path, int size);
    int tailleFichier();

signals:
    void finished();
    void error(QString err);

public slots:
    void process();
    
};

#endif // SERVEURTHREAD_H
