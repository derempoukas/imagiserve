/******************************************************************************
HtmlPage renderes the HTML view for the PageModel objects. For every different
PageMoadel::ModelType a corresponding HtmlPage i created.

Author: Michail Derempoukas m.derempoukas@gmail.com

******************************************************************************/

#ifndef PAGEVIEW_H
#define PAGEVIEW_H
#include <QString>
#include <QFileInfoList>
#include "pagemodel.h"


class HtmlPage
{
public:
    HtmlPage(PageModel& model);
    QString render() const;

protected:
    QString renderSingleImageView() const;
    QString renderDirView() const;
    QString renderHead() const;
    void renderBody() const;

    PageModel& m_model;
};

#endif // PAGEVIEW_H
