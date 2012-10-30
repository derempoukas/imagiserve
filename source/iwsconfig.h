/******************************************************************************
IwsConfig class contains application wide configurations globals such as
server ip , port , server timeout limits .. etc. Most of them are hardcoded here
but the most important can also be read from the file "configuration.xml".

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/
#ifndef IWSCONFIG_H
#define IWSCONFIG_H

#include <QHostAddress>
#include <QStringList>




class IwsConfig:QObject
{
public:
    static IwsConfig* getInstance(QObject *parent=0);
    ~IwsConfig();

    //server
    int connectionTimeout() { return m_connectionTimeOut;}
    int port(){return m_port;}
    const QString webRoot() const {return m_webRoot;}

    //view  (HTML page)
    QHostAddress  address()  {return  m_address;}
    QString docType()  {return m_docType;}
    QString meta()  {return m_meta;}

    QString dirViewCss()  { return m_dirViewCss;}  //css for the directory view pages
    QString singleImageCss() { return m_singleImageCss; } //css for single image
    //view pages
    int thumbnailHeight() {return m_thumbnailHeight ; }
    QString iconsPath(){ return m_iconsPath;}

    QString resourceDirPath() { return m_resourceDirPath;}
    QStringList imageTypes() { return m_imageTypes;}
    QStringList imageFileFilter() { return m_imageFileFilter;}


private:
    void readFromXml();
    QString readCssFromFile(QString fileName);
    void populateImageFileFilter();

    IwsConfig(QObject *parent);
    static IwsConfig* m_instance;

    int  m_connectionTimeOut;
    int  m_port;
    int  m_thumbnailHeight;
    QHostAddress  m_address;
    QString m_webRoot;

    QString m_meta;
    QString m_docType;
    QString m_dirViewCss;         //the css appended at all directory view pages
    QString m_singleImageCss;     //the css appended at alla single image view pages

    QString m_iconsPath;
    QString m_resourceDirPath;
    QStringList m_imageTypes;     // the suffixes of all acceptable image types
    QStringList m_imageFileFilter;

};


#endif // IWSCONFIG_H
