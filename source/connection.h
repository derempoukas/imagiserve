/******************************************************************************
Class Connection represents each unique connection accepted by the server at
the TCP/IP level. For every incoming connection a Connection object in a
seperate thread is created.

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QThread>
#include <QTcpSocket>


class Connection : public QThread
{


public:

     Connection(int descriptor ,  QObject* parent);
     void run();
protected:
     void sendResponse(QByteArray &responose,QTcpSocket &socket);
     void waitForRequest(QTcpSocket &socket);
     int m_descriptor;

};

#endif // CONNECTION_H
