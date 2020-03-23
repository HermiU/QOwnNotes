#include "PDF_DocumentWidget.h"

#include <QDebug>

PDF_DocumentWidget::PDF_DocumentWidget(QWidget *parent)
    : QLabel(parent)
{
    PDF_Document = nullptr;
    CurrentPage  = INVALID_PAGE;
    ScaleFactor  = 1.0;
    Rotation     = Poppler::Page::Rotate0;

    setAlignment(Qt::AlignCenter);
}

PDF_DocumentWidget::~PDF_DocumentWidget()
{
    delete PDF_Document;
}

qreal PDF_DocumentWidget::scale() const
{
    return ScaleFactor;
}

int PDF_DocumentWidget::page() const
{
    return CurrentPage;
}

int PDF_DocumentWidget::pages() const
{
    return PDF_Document->numPages();
}

QSize PDF_DocumentWidget::imageSize(void) const
{
    QSize rc = QSize(100, 100);

    if (PDF_Document != nullptr)
    {
        if ( CurrentPage > 0 )
        {
            rc = PDF_Document->page(CurrentPage-1)->pageSize();
        }
    }

    return rc;
}

bool PDF_DocumentWidget::setDocument(const QString filename)
{
    if (PDF_Document != nullptr)
    {
        delete PDF_Document;
    }

    PDF_Document = Poppler::Document::load(filename);
    if ( PDF_Document )
    {
        //qDebug() << "Read PDF file" << filePath;
        PDF_Document->setRenderHint(Poppler::Document::Antialiasing);
        PDF_Document->setRenderHint(Poppler::Document::TextAntialiasing);
        CurrentPage = 1;
    }

    return PDF_Document != nullptr;
}

void PDF_DocumentWidget::showPage(const int page)
{
    if ( PDF_Document != nullptr )
    {
        CurrentPage = page;

        QImage image = PDF_Document->page(CurrentPage-1)->renderToImage(ScaleFactor * physicalDpiX(),
                                                                        ScaleFactor * physicalDpiY(),
                                                                        -1, -1, -1, -1,
                                                                        Rotation);
        setPixmap(QPixmap::fromImage(image));
    }
}

void PDF_DocumentWidget::setPage(const int new_page)
{
    if ( new_page != CurrentPage )
    {
        showPage(new_page);
    }
}

void PDF_DocumentWidget::scalePage(const qreal scale)
{
    if ( ScaleFactor != scale )
    {
        qDebug() << "Scale:" << scale;
        ScaleFactor = scale;
        showPage(CurrentPage);  /* Redraw */
    }
}
