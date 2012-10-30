/******************************************************************************
Handles a get request. It can handle directory listings (html), single image
views (html) and plain image requests (just the image ) whether those images
exist under <webRoot> or they are "icons" for Directory representation and
navigation.The "icon" files can be of any type IwsConfig::imageTypes()returns
and not necesseraly actual .ico files.It also performs realtime scaling of the
images when they are requested as a part of directory view page.

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/

#ifndef GETHANDLER_H
#define GETHANDLER_H
#include <QDir>
#include <QUrl>
#include "htmlpage.h"

class GetHandler
{
public:

    enum ResponseType{
         DirectoryPage,    //listing of all the images in the directory
         SingleImagePage,  //page displaying one image at a time
         PlainImageFile,   //the actual image
         ThumbnailFile,    //thumbnail preview of the image (for use in the directory pages)
         PlainIconFile,    // an image stored in the resources folder used for icon depiction
         NotFoundPage
    };

    GetHandler(const QString  &resourcePath);
    QByteArray serveResponse();
    ResponseType getResponseType() { return m_responseType;}


private:

    void determineResponseType();

    QByteArray serveDirectoryPage();
    QByteArray serveSingleImagePage();
    QByteArray servePlainImageFile();      //response containing only header and the image
    QByteArray serveThumbnail(int height); //scales images to height and servers them back
    QByteArray servePlainIconFile();       //response contain only header and the image

    QByteArray serveNotFoundPage();        //full not found response with header and html body
    QByteArray header();


    QString m_resourcePath;
    QString m_iconImageFileName;
    QString m_singleImageViewName;
    QString m_thumbnailPath;

    QString m_absoluteResourcePath;



    ResponseType m_responseType;

};

#endif // GETHANDLER_H
