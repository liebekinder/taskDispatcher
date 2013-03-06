#-------------------------------------------------
#
# Project created by QtCreator 2013-02-20T22:40:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = taskDispatcher
TEMPLATE = app


SOURCES += main.cpp\
        fenetreprincipale.cpp \
    controleur.cpp \
    serveur.cpp \
    client.cpp \
    serveurthread.cpp

HEADERS  += fenetreprincipale.h \
    controleur.h \
    serveur.h \
    client.h \
    Parametres.h \
    serveurthread.h

OTHER_FILES += \
    junk.txt
