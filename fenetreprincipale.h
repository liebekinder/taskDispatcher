#ifndef FENETREPRINCIPALE_H
#define FENETREPRINCIPALE_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QFormLayout>
#include <QSpinBox>

class Controleur;

class FenetrePrincipale : public QMainWindow
{
    Q_OBJECT
    
public:
    FenetrePrincipale(QWidget *parent = 0);
    ~FenetrePrincipale();

    //fields
    QPushButton * client;
    QPushButton * server;
    QSpinBox * port;
    Controleur * c ;

    //client

        QWidget * clientContener;
        QLabel * cName;

    //server

        QWidget * serverContener;
        QLabel * sName;

    //methods

        void setClientGUI();
        void setServerGUI();
};

#endif // FENETREPRINCIPALE_H
