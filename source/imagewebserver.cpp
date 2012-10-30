
#include "imagewebserver.h"
#include "connection.h"



ImageWebServer::ImageWebServer():QTcpServer()
{
}


void ImageWebServer::incomingConnection( int socketDescriptor)
{
    Connection  *inConn = new Connection( socketDescriptor, this );


    connect( inConn, SIGNAL(finished()),inConn,SLOT(deleteLater()));

    inConn->start();
}
