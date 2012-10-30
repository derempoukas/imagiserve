#include <QApplication>
#include <QDir>
#include <QDomDocument>
#include "iwsconfig.h"


IwsConfig* IwsConfig::m_instance=0;

IwsConfig::~IwsConfig(){     
}

IwsConfig::IwsConfig(QObject *parent):QObject(parent)
{
    QString appPath = QCoreApplication::applicationDirPath();
    m_connectionTimeOut = 5 * 1000;
    m_port=80;
    m_address = QHostAddress("127.0.0.1");
    m_meta  = QString("<meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\"/>");
    m_docType = QString("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\"\
http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">");
    m_imageTypes << "jpeg" << "jpg" << "png" << "bmp" << "gif";
    m_imageTypes << "JPEG" << "JPG" << "PNG" << "BMP" << "GIF";
    populateImageFileFilter();

    QDir dir(appPath);

    if (!dir.cd("../webRoot")) {
        IwsConfig::m_webRoot = appPath;
    }
    else{
        IwsConfig::m_webRoot=dir.absolutePath();
    }

    dir.setPath(appPath);
    if (!dir.cd("../resources"))
    {
        m_resourceDirPath = appPath;
    }
    else{
        m_resourceDirPath=dir.absolutePath();
    }

    readFromXml();

    qDebug() << "================================================";
    qDebug() << "initializing server with the following settings"<<endl;
    qDebug() <<  "IP Address............:" << m_address.toString();
    qDebug() <<  "Port .................:" << m_port;
    qDebug() <<  "webRoot path .........:" << m_webRoot;
    qDebug() <<  "resource dir path.....:" << m_resourceDirPath;
     qDebug() << "================================================";

    m_dirViewCss = readCssFromFile(m_resourceDirPath+"/dir-view.css");
    m_singleImageCss = readCssFromFile(m_resourceDirPath+"/single-image-view.css");
}



void IwsConfig::readFromXml(){

    QFile f("configuration.xml");
    if (!f.open(QIODevice::ReadOnly)){
        qDebug() << "unable to open "<< f.fileName();
        return;
    }
    QDomDocument doc;
    QString error;
    if(!doc.setContent(&f,&error)){
        qDebug() <<  "the following error occurred while reading " << f.fileName();
        qDebug() <<  error ;
    }

    QDomElement currentElement;
    QDomNode config= doc.firstChildElement("CONFIGURATION");

    currentElement = config.firstChildElement("WEBROOT").toElement();
    if (/*currentElement.isText() && */currentElement.text()!=""){
        QFileInfo fi(currentElement.text());
        if (fi.exists() && fi.isDir())
            m_webRoot = currentElement.text();
    }
    currentElement = config.firstChildElement("IP").toElement();
    if (/*currentElement.isText() &&*/ currentElement.text()!=""){
        if (!m_address.setAddress( currentElement.text())){
            qDebug() << "configuration xml contains invalid ip " ;
            qDebug() << "continuing with 127.0.0.1 " ;
            m_address.setAddress("127.0.0.1");
        }
    }
    currentElement = config.firstChildElement("PORT").toElement();
    if (/*currentElement.isText() && */currentElement.text()!="" ){
        int port = currentElement.text().toInt();
        if (port>=0  && port<=65535 ){
            m_port=port;
        }
     }
    currentElement = config.firstChildElement("THUMBNAIL-HEIGHT").toElement();
    if (/*currentElement.isText() &&*/ currentElement.text()!="" ){
        int height = currentElement.text().toInt();
        if (((height >= 100)  && (height <=300)) || (height = -1)){
            m_thumbnailHeight=height;}
        }
    }




QString IwsConfig::readCssFromFile(QString fileName){

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
        qDebug()<< "failed to read css from "<<file.fileName();
    }
    QTextStream in(&file);
    return in.readAll();// small css files only, no need to read line by line
}

void IwsConfig::populateImageFileFilter(){
    for (int i = 0 ; i<m_imageTypes.size() ; i++){
        QString imgType =m_imageTypes.at(i);
        m_imageFileFilter.append(imgType.prepend("*."));
    }
}

IwsConfig*  IwsConfig::getInstance(QObject* parent){

    if (m_instance)
        return m_instance;

    IwsConfig::m_instance = new IwsConfig(parent);
    return IwsConfig::m_instance ;
}


