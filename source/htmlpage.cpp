#include "htmlpage.h"
#include "iwsconfig.h"


HtmlPage::HtmlPage(PageModel& model):m_model(model){
}

QString HtmlPage::render() const{
    QString htmlPage;
    switch (m_model.modelType()){
    case PageModel::DirectoryModel:
        htmlPage = renderDirView();
        break;
    case PageModel::SingleImageModel:
        htmlPage = renderSingleImageView();
        break;
    }
    return htmlPage;
}

QString HtmlPage::renderDirView() const{

    QString head,body;
    head=renderHead();
    body.clear();
    body.append("<body>");
    body.append("<div class=\"header-left\"> &nbsp</div>");
    body.append(QString("<div class=\"header-center\"><h1>%1</h1></div>").arg(m_model.title()));
    body.append("<div class=\"header-right\"> &nbsp</div>");
    body.append("<div class=\"left-col\"> &nbsp</div>" );

    body.append("<div class=\"center-col\">");
    QList<Item> items = m_model.items();
    for (int i=0 ; i<items.count() ; i++){

        body.append("<div class=\"img\">");
        body.append(QString("<a  href=\"%1\">").arg(items.at(i).href()) );
        body.append(QString("<img src=\"%1\" alt=\"%2\" />").arg(items.at(i).imgSrc()).arg(items.at(i).description()));
        body.append(QString("<div class=\"desc\">%1</div></a></div>").arg(items.at(i).description()));
    }
    body.append("</div>");
    body.append("<div class=\"right-col\">&nbsp</div>" );
    body.append("</body></html>");
    return head+body;
}

QString HtmlPage::renderSingleImageView() const{

    QString head,body;
    head=renderHead();
    body.clear();
    body.append("<body>");
    body.append("<div class=\"btn-left\"> &nbsp</div>");

 body.append("<div class=\"btn-center\">");
    QList<Item> items = m_model.items();
    for (int i=0 ; i<items.count()-1 ; i++){
        body.append(QString("<div class=\"nav\">")); //.arg(divClass));
        body.append(QString("<a  href=\"%1\">").arg(items.at(i).href()) );
        body.append(QString("<img src=\"%1\" alt=\"%2\" />").arg(items.at(i).imgSrc()).arg(items.at(i).description()));
        body.append(QString("<div class=\"desc\">%1</div></a></div>").arg(items.at(i).description()));
    }
    body.append("</div>");

    body.append("<div class=\"img\">");
    int last = items.size()-1;

    body.append(QString("<a  href=\"%1\">").arg(items.at(last).href()) );
    body.append(QString("<img src=\"%1\" alt=\"%2\" />").arg(items.at(last).imgSrc()).arg(items.at(last).description()));
    body.append(QString("<div class=\"desc\">%1</div></a>").arg(items.at(last).description()));

    body.append("</div>" );
    body.append("</body></html>");
    return head+body;
}


QString  HtmlPage::renderHead() const{
    QString htmlHead;
    htmlHead.clear();
    IwsConfig *cfg = IwsConfig::getInstance();
    htmlHead.append(cfg->docType());
    htmlHead.append(QString("<html><head><title>%1</title>").arg(m_model.title()));
    htmlHead.append(cfg->meta());
    htmlHead.append(m_model.css());
    htmlHead.append("</head>");
    return htmlHead;
}





