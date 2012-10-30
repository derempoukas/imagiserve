/*******************************************************************************
HttpProtocol is where the first level of the http protocol processing is
performed the server request is parsed by HttpProtocol::getResponse(). Since
this is a "get"  intensive application the get requests are handled in a
dedicated class GetHandler ,this  way we also avoid accessing the filesystem
from HttProtocol and any type of I/O takes place only in Connections class for
network and specialiazed Handler classes for file I/O.

Author: Michail Derempoukas m.derempoukas@gmail.com

*******************************************************************************/
#ifndef HTTPPROTOCOL_H
#define HTTPPROTOCOL_H
#include <QByteArray>
#include <QString>
#include <QStringList>


class HttpProtocol
{
public:

    enum Request{
        Get=0,
        Head,
        Post,
        Put,
        Delete,
        Trace,
        Connect
    };

    HttpProtocol();

    QByteArray getResponse(const QString &request);
    bool completeRequest(const QStringList &partialReq);

private:


    QByteArray respondBadRequest(); //400 status code
    QByteArray respondNotImplemented(); //501 HTTP status code

    QString m_request;
    QByteArray m_responseHeader;
    QByteArray m_responseEntity;

    QStringList m_requestNames;

};

#endif // HTTPPROTOCOL_H
