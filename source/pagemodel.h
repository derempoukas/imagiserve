/******************************************************************************
PageModel  is responsible for the creation of HTML page models from URLs . No actual
HTML is created here just the contents of the page.The class HtmlPage takes care
of rendering the model to actual HTML view.

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/
#ifndef PAGEMODEL_H
#define PAGEMODEL_H
#include "item.h"



class PageModel
{
public:
    enum ModelType{
        DirectoryModel,     //a page model representing a directory listing with thumbnails
        SingleImageModel    //a page model representing one image and navigation buttons
    };

    PageModel(const QString& resourceUrl , const ModelType type);

    const QString title(){return  m_title;}
    const QList<Item>& items() {return m_items;}
    const QString css() {return m_css;}
    const ModelType modelType() { return m_ModelType;}

protected:
    void buildDirectoryModel();
    void buildSingleImageModel();
    void populateItems();

    const ModelType m_ModelType;
    QList<Item> m_items;

    QString m_resourceUrl;
    QString m_title;
    QString m_css; //
};

#endif // PAGEMODEL_H
