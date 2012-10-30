#include <QUrl>
#include <QRegExp>
#include <QDebug>
#include <QImage>
#include <QBuffer>
#include <QPixmap>
#include "GetHandler.h"
#include "iwsconfig.h"

GetHandler::GetHandler(const QString &resourceUrl)
{
    QUrl theUrl(resourceUrl);

    m_iconImageFileName = QUrl::fromPercentEncoding(theUrl.queryItemValue("icon-file").toUtf8());
    m_singleImageViewName =  QUrl::fromPercentEncoding((theUrl.queryItemValue("single-image-view").toUtf8()));
    m_thumbnailPath = QUrl::fromPercentEncoding((theUrl.queryItemValue("thumbnail").toUtf8()));
    m_resourcePath=QUrl::fromPercentEncoding(resourceUrl.toUtf8());


    IwsConfig *cfg = IwsConfig::getInstance();
    qDebug() << "resourcePath received: " << resourceUrl;
    m_absoluteResourcePath = cfg->webRoot()+"/"+m_resourcePath ;

}

void GetHandler::determineResponseType(){
    IwsConfig *cfg = IwsConfig::getInstance();
    QFileInfo resource(m_absoluteResourcePath);
    if (resource.exists()){
        if (resource.isDir()){ //directory
            m_responseType = DirectoryPage ;
        }
        else if (cfg->imageTypes().contains(resource.suffix())) { // image suffix

            m_responseType = PlainImageFile;  //actual image
        }
    }
    else
    { if (m_iconImageFileName!="")
        m_responseType = PlainIconFile;   // ui "icon" image
        else
            if (m_singleImageViewName!="")
                m_responseType = SingleImagePage; //single image html page
        else if (m_thumbnailPath!="")
            m_responseType=ThumbnailFile;
        else
            m_responseType=NotFoundPage; //resource not found 404
    }
}


QByteArray GetHandler::serveDirectoryPage(){
    QByteArray response;
    QDir dir(m_absoluteResourcePath);
    if (!dir.exists()) {
        return serveNotFoundPage();
    }

    PageModel model(m_resourcePath,PageModel::DirectoryModel);
    HtmlPage html(model);
    response.append(header());
    response.append(html.render().toUtf8());

    return response;
}


QByteArray GetHandler::serveSingleImagePage(){

    QByteArray response;
    IwsConfig *cfg = IwsConfig::getInstance();

    QFileInfo file(cfg->webRoot()+m_singleImageViewName);
    if (!file.exists()) {
        return serveNotFoundPage();
    }

    PageModel model(m_singleImageViewName,PageModel::SingleImageModel);
    HtmlPage html(model);
    response.append(header());
    response.append(html.render().toUtf8());

    return response;
}


QByteArray GetHandler::servePlainIconFile(){
    QByteArray response;
    IwsConfig *cfg = IwsConfig::getInstance();
    QString imageAbsolutePath  = cfg->resourceDirPath().remove(QRegExp("/$"))
                                 +"/"+m_iconImageFileName;

    QFile image(imageAbsolutePath);
    if (!image.exists()) {
        return serveNotFoundPage();
    }
    image.open(QIODevice::ReadOnly);
    QByteArray body= image.readAll();
    qDebug() << "serving the following file : "<< m_absoluteResourcePath.toUtf8();
    image.close();

    QByteArray header =GetHandler::header();
    response+=header;
    response+=body;
    return response;
}


QByteArray GetHandler::servePlainImageFile(){
    QByteArray response;

    QFile image(m_absoluteResourcePath);
    if (!image.exists()) {
        return serveNotFoundPage();
    }
    image.open(QIODevice::ReadOnly);
    QByteArray body= image.readAll();
    image.close();
    QByteArray header =GetHandler::header();

    response+=header;
    response+=body;

    return response;
}

QByteArray GetHandler::serveThumbnail(int height){
    IwsConfig *cfg = IwsConfig::getInstance();

    QByteArray response;
    QByteArray suffix = QFileInfo(m_thumbnailPath).suffix().toAscii();

    if ((suffix=="gif") || (suffix =="GIF")){ // no risize for gif
        m_absoluteResourcePath =cfg->webRoot()+m_thumbnailPath;
        return servePlainImageFile();
    }

    QImage pic,thumb;
    if (!pic.load(cfg->webRoot()+m_thumbnailPath, suffix))
        return serveNotFoundPage();
    if (height!=-1 )
        thumb = pic.scaledToHeight(height);
    else
        thumb = pic;
    QByteArray header =GetHandler::header();
    QBuffer smallImage;
    thumb.save(&smallImage,suffix);
    response+=header;
    response+=smallImage.data();

    return response;

}



QByteArray GetHandler::header() {
    QString contentType,responseDescription;

    switch (m_responseType){
    case DirectoryPage:
        responseDescription="HTTP/1.1 200 OK\r\n";
        contentType="Content-Type:text/html; charset=utf-8 \r\n";
        break;
    case SingleImagePage:
        responseDescription="HTTP/1.1 200 OK\r\n";
        contentType="Content-Type:text/html; charset=utf-8 \r\n";
        break;
    case  NotFoundPage:
        responseDescription="HTTP/1.1 404 Not Found\r\n";
        contentType="Content-Type:text/html;charset=utf-8 \r\n";
        break;
    case PlainImageFile:
    case ThumbnailFile:

    case PlainIconFile:
        QFileInfo resource(m_absoluteResourcePath);
        responseDescription="HTTP/1.1 200 OK\r\n";
        contentType=QString("Content-Type: image/%1\r\n").arg(resource.suffix());
    }

    QByteArray header;
    header.append(responseDescription.toUtf8());
    header.append(contentType.toUtf8());
    header.append("\r\n");
    return header;

}

QByteArray GetHandler::serveNotFoundPage(){
    QString header;
    header.append("HTTP/1.1 404 Not Found\r\n");
    header.append("Connection: close\r\n");
    header.append("Content-Type:text/html;charset=utf-8\r\n\r\n");
    header.append("<html><head><title>404 Not Found</title>");
    header.append("</head><body>");
    header.append(QString("<h3> %1 Not Found</h3>").arg(m_resourcePath.remove(QRegExp("^/"))));
    header.append("<p> <h2>404 Not Found<h2></p>");
    header.append("</body></html>");

    return header.toUtf8();

}

QByteArray GetHandler::serveResponse(){
    determineResponseType();
    QByteArray response;
    switch (m_responseType){
    case DirectoryPage:{
            response = serveDirectoryPage();
            break;
        }
    case SingleImagePage:{
            response = serveSingleImagePage();
            break;
        }
    case PlainImageFile:{
            response = servePlainImageFile();
            break;
        }
    case PlainIconFile:{
            response = servePlainIconFile();
            break;
        }
    case ThumbnailFile:{
            IwsConfig *cfg = IwsConfig::getInstance();
            response = serveThumbnail(cfg->thumbnailHeight());
            break;
        }
    case  NotFoundPage:{
            response = serveNotFoundPage();
            break;
        }
    }
    return response;

}


