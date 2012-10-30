#include <QTcpSocket>
#include <QDebug>
#include <QByteArray>
#include "connection.h"
#include "httpprotocol.h"
#include "IwsConfig.h"

Connection::Connection(int descriptor ,  QObject* parent) :
        QThread(parent)

{
    m_descriptor = descriptor;

}

void Connection::run()
{
    QTcpSocket socket;


    if( !socket.setSocketDescriptor(m_descriptor))
    {
        qDebug(socket.errorString().toUtf8());
        return;
    }
    waitForRequest(socket);

}


// from this point will use the synchronous socket approach in order to
// keep control of the response/request pair simple
void Connection::waitForRequest(QTcpSocket &socket){
    qDebug("New incoming  connection accepted");

    QStringList partialReq,request;

    HttpProtocol http;
    IwsConfig* cfg = IwsConfig::getInstance();

    while (socket.waitForReadyRead(cfg->connectionTimeout())){
        while (socket.canReadLine()){
            QString line(socket.readLine());
            partialReq.append(line);
        }
        if (http.completeRequest(partialReq)){
            request = partialReq ;
            break;
        }
    }

    if (request.size()>0 ){
        QByteArray response = http.getResponse(request.join(""));
        socket.write(response);

    }
    socket.disconnectFromHost();
    if(socket.state()!=QAbstractSocket::UnconnectedState)
        socket.waitForDisconnected();
}
