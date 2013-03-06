#include "fenetreprincipale.h"
#include "controleur.h"

FenetrePrincipale::FenetrePrincipale(QWidget *parent)
    : QMainWindow(parent)
{
    c = new Controleur(this);
    this->setMaximumSize(200,100);
    this->setWindowTitle("Task Dispatcher");

    QWidget * contener = new QWidget();
    QVBoxLayout * contenerL = new QVBoxLayout();
        client = new QPushButton("client");
        server = new QPushButton("server");
        QFormLayout * form = new QFormLayout();
            QLabel * portI = new QLabel("port");
            port = new QSpinBox();
                port->setMaximum(59999);
                port->setMinimum(0);
                port->setValue(5000);
            form->addRow(portI,port);
        contenerL->addWidget(client);
        contenerL->addWidget(server);
        contenerL->addLayout(form);
    contener->setLayout(contenerL);

    this->setCentralWidget(contener);

    connect(client,SIGNAL(pressed()),c,SLOT(client()));
    connect(server,SIGNAL(pressed()),c,SLOT(server()));

}

FenetrePrincipale::~FenetrePrincipale()
{
    
}

void FenetrePrincipale::setClientGUI()
{
    clientContener = new QWidget();
    cName = new QLabel("client!   "+QString::number(c->getPort()),clientContener);
    this->setCentralWidget(clientContener);
    c->runClient();
}

void FenetrePrincipale::setServerGUI()
{
    serverContener = new QWidget();
    sName = new QLabel("server!  "+QString::number(c->getPort()),serverContener);
    this->setCentralWidget(serverContener);
    c->runServer();
}
