#include "pagemodel.h"




PageModel::PageModel(const QString& resourceUrl , const ModelType modelType):m_ModelType(modelType)
{
    m_resourceUrl = resourceUrl;
    populateItems();
}



void PageModel::populateItems(){

    switch (m_ModelType){
    case DirectoryModel:
        buildDirectoryModel();
        break;
    case SingleImageModel:
        buildSingleImageModel();
        break;
    }

}



void PageModel::buildDirectoryModel(){
    IwsConfig *cfg = IwsConfig::getInstance();

    m_css=cfg->dirViewCss();

    QDir dir(cfg->webRoot()+m_resourceUrl);
    if (dir.exists()){
        m_title = QString("Images in : ")+m_resourceUrl;
        dir.setFilter(QDir::Dirs);

        QFileInfoList dirInfoList=  dir.entryInfoList(QDir::Dirs ,QDir::Name);
        for (int i=0; i < dirInfoList.size() ; i++){
            QString curDirName = dirInfoList.at(i).fileName();
            if(curDirName!="." && !(m_resourceUrl=="/" && curDirName=="..")){
                Item item;
                QString iconFileName;
                if (curDirName==".."){
                    iconFileName = "back-folder.png";
                    item.m_description ="back";
                }
                else{
                    iconFileName ="folder.png";
                    item.m_description = dirInfoList.at(i).fileName();
                }
                item.m_href = m_resourceUrl.remove(QRegExp("/$"))+"/"+dirInfoList.at(i).fileName();
                item.m_imgSrc = QString("?icon-file=")+iconFileName;
                item.m_itemType = Item::ItemNav;
                m_items.append(item);
            }
        }
        QDir images(cfg->webRoot()+m_resourceUrl);
        images.setFilter(QDir::Files);

        images.setNameFilters(cfg->imageFileFilter());
        QFileInfoList imageInfoList=  images.entryInfoList(QDir::Files,QDir::Name);
        for (int i=0; i < imageInfoList.size() ; i++){

            Item item;
            item.m_description = imageInfoList.at(i).fileName();
            item.m_href = "?single-image-view="+m_resourceUrl.remove(QRegExp("/$"))+"/"+imageInfoList.at(i).fileName();
            item.m_imgSrc ="?thumbnail="+m_resourceUrl.remove(QRegExp("/$"))+"/"+imageInfoList.at(i).fileName();
            item.m_itemType = Item::ItemImage;
            m_items.append(item);
        }
    }
}


void PageModel::buildSingleImageModel(){

    IwsConfig *cfg = IwsConfig::getInstance();
    m_css = cfg->singleImageCss();

    QString absoluteFilePath = cfg->webRoot()+m_resourceUrl; // normaly this should contain
                                                             // a filename at the end
    QString relResourcePath; // no file name here
    QString fileName;

    QFileInfo imageFi(absoluteFilePath);
    if (! imageFi.exists())
        return;
    if (imageFi.isFile()){
        relResourcePath =  m_resourceUrl.left( m_resourceUrl.lastIndexOf(imageFi.fileName()));
        fileName = imageFi.fileName();
    }
    else
         relResourcePath = m_resourceUrl;

    QDir images =  imageFi.absoluteDir();

    images.setFilter(QDir::Files);


    images.setNameFilters(cfg->imageFileFilter());
    QFileInfoList imageInfoList=  images.entryInfoList(QDir::Files,QDir::Name);
    int p = imageInfoList.indexOf(imageFi);
    if (p == -1) return;
    int prv =  ((p==0) ?   imageInfoList.size()-1 :  p-1); // backward rotate
    int nxt = (p+1)%imageInfoList.size(); //forward rotate

    Item prev;
    prev.m_description = "Previous image";
    prev.m_href = "?single-image-view="+relResourcePath+imageInfoList.at(prv).fileName().toUtf8();
    prev.m_imgSrc = "?icon-file=previous.png";
    prev.m_itemType = Item::ItemNav;
    m_items.append(prev);

    Item upDir;
    upDir.m_description = "Return to  "+ relResourcePath;;
    upDir.m_href =relResourcePath; // m_resourceUrl.left(m_resourceUrl.lastIndexOf(imageFi.fileName()));
    upDir.m_imgSrc = "?icon-file=folder.png";
    upDir.m_itemType = Item::ItemNav;
    m_items.append(upDir);

    Item next;
    next.m_description = "Next image";
    next.m_href = "?single-image-view="+relResourcePath+imageInfoList.at(nxt).fileName();
    next.m_imgSrc = "?icon-file=next.png";
    next.m_itemType = Item::ItemNav;
    m_items.append(next);

    Item theImage;
    theImage.m_description = imageInfoList.at(p).fileName();
    theImage.m_href = m_resourceUrl;
    theImage.m_imgSrc = m_resourceUrl;
    theImage.m_itemType = Item::ItemImage;
    m_items.append(theImage);

    m_title = imageInfoList.at(p).fileName();
}





