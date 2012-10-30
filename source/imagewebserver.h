/******************************************************************************
 ImageWebServer is the QTcpServer that listens for new incomming connections
 and for every connection creates a new Connection object in a seperate thread

 Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/

#ifndef SERVER_H
#define SERVER_H
#include <QTcpServer>
#include "iwsconfig.h"

class ImageWebServer: public QTcpServer
{


public:
    ImageWebServer();

protected:
    void incomingConnection( int descriptor);
};

#endif // SERVER_H
