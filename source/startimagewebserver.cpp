
//#include <QCoreApplication>
#include "imagewebserver.h"
#include "iostream"
#include <QApplication>

int main ( int argc , char **argv)
{

    QApplication app(argc, argv,false);
    IwsConfig* cfg = IwsConfig::getInstance(&app);
    ImageWebServer iws;


    if (iws.listen(cfg->address(),cfg->port())){
        qDebug() << endl<<"server is up and running " ;
        QString port = cfg->port()==80 ?  "" : ":"+QString::number(cfg->port(),10);
        qDebug() << endl<<"open your browser and type at the address field: "<< cfg->address().toString()+port;
        qDebug() << endl<<"make sure you have some pictures to view in :"<< cfg->webRoot();
    }
    else{
        qDebug("failed to start server");
        qDebug("check whether another server is running with the above settings and make the apropriate changes in configuration.xml");
    }
    return  app.exec();
}

