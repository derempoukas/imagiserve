/******************************************************************************
PageItem is the building block of a PageModel. The HtmlPage produces output for
every item in the model.

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/
#ifndef ITEMMODEL_H
#define ITEMMODEL_H
#include <QDir>
#include "iwsconfig.h"

class PageModel;

class Item
{
    friend class PageModel;
public:
    enum ItemType{
        ItemImage, // the actual images
        ItemNav    // "icons" for navigation and ui usage
    };

    const QString &imgSrc() const{return m_imgSrc;}
    const QString &href()  const { return m_href;}
    const QString &description() const{return m_description;}
    const ItemType itemType() const{return m_itemType;}

private:
    QString m_imgSrc;       //the relative path of the image or a get
                            //query string that  returns the actual image
    QString m_href;         //the link that this item maps to
    QString m_description;  // the label of the item
    ItemType m_itemType;

};

#endif // ITEMMODEL_H
