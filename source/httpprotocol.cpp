#include <QRegExp>
#include <QDir>
#include <QFile>
#include <QtDebug>
#include "httpprotocol.h"
#include "gethandler.h"


HttpProtocol::HttpProtocol(){
    m_requestNames<< "GET" <<"HEAD" << "POST" << "PUT"<<\
            "DELETE"<<"TRACE"<<"CONNECT";
}


// Checks whether the last line contains only \r\n
//return true when the whole request has been received
bool HttpProtocol::completeRequest(const QStringList &partialReq){
    if (partialReq.size()<2 ){
        return false;
    }

    QString last = partialReq.at((partialReq.size()-1));
    QRegExp rx("^\\r\\n");

    return rx.exactMatch(last);

}





QByteArray HttpProtocol::getResponse(const QString &request){

    QStringList tokens= request.split(QRegExp("\\s+"));
    QString reqName= tokens.at(0).toUpper();
    QByteArray response;
    switch (m_requestNames.indexOf(reqName)){
    case Get :{
        QString resourceUrl( tokens.at(1));
        GetHandler handler(resourceUrl);
        response =  handler.serveResponse();
        break;
    }
    case  Head :
    case  Post :
    case  Put  :
    case  Delete :
    case  Trace :
    case  Connect:{
        response =  respondNotImplemented();
        break;
    }
    default:
        response = respondBadRequest();
    }
    return response;
}



QByteArray HttpProtocol::respondNotImplemented(){
    QString header;
    header.append("HTTP/1.1 501 Not Implemented \r\n");
    header.append("Connection: close\r\n");
    header.append("Content-Type: text/html\r\n\r\n");

    QString body;
    body.append("<html><head><title>501 Not Implemented </title></head><body>");
    body.append("<p><h1>501 Not Implemented<h1></p>");
    body.append("</body></html>");
    return header.toUtf8()+body.toUtf8();

}

QByteArray HttpProtocol::respondBadRequest(){
    QString header;
    header.append("HTTP/1.1 400 Bad Request \r\n");
    header.append("Connection: close\r\n");
    header.append("Content-Type: text/html\r\n\r\n");

    QString body;
    body.append("<html><head><title>400 Bad Request</title></head>");
    body.append("<body><p><h1>400 Bad Request<h1></p></body></html>");
    return header.toUtf8()+body.toUtf8();
}




