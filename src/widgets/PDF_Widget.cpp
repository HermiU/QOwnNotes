#include "PDF_Widget.h"

#include <QFileInfo>
#include <QWheelEvent>
#include <QVBoxLayout>
#include <QScrollArea>

#include "PDF_DocumentWidget.h"

PDF_Widget::PDF_Widget(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    ScrollArea = new QScrollArea(this);
    ScrollArea->setBackgroundRole(QPalette::Dark);
    ScrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    ScrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
    ScrollArea->setWidgetResizable( true );

    DocumentWidget = new PDF_DocumentWidget();
    ScrollArea->setWidget(DocumentWidget);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(ScrollArea);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    ScaleFactors << 0.25 << 0.33 << 0.5 << 0.66 << 0.75 << 1.0 << 1.25 << 1.5 << 2.0 << 4.0 << 8.0 << 16.0;
    ScaleIndex = calcScaleIndexFromScaleFactor(1.0);
    SelectedPage = 1;
}

void PDF_Widget::showPDF_File(QString filename)
{
    QFileInfo* fileinfo = new QFileInfo(filename);

    if ( fileinfo->isFile() )
    {
        if ( DocumentWidget->setDocument(filename) )
        {
            SelectedPage = 1;  /* Show first page */

            scaleToFit();
        }
    }
}

void PDF_Widget::scaleDocumentSlot(int index)
{
    if ( (index >= 0) && (index < ScaleFactors.size()) )
    {
        ScaleIndex = index;
        DocumentWidget->scalePage(ScaleFactors[index]);
    }
}
void PDF_Widget::zoomInSlot(void)
{
    scaleDocumentSlot(ScaleIndex+1);
}
void PDF_Widget::zoomOutSlot(void)
{
    scaleDocumentSlot(ScaleIndex-1);
}

void PDF_Widget::setPageSlot(int page)
{
    if ( (page > 0) && (page <= DocumentWidget->pages()) )
    {
        SelectedPage = page;
        DocumentWidget->setPage(page);
    }
}
void PDF_Widget::upPageSlot(void)
{
    setPageSlot(SelectedPage-1);
}
void PDF_Widget::downPageSlot(void)
{
    setPageSlot(SelectedPage+1);
}

void PDF_Widget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void PDF_Widget::wheelEvent(QWheelEvent *event)
{
    if ( ! (event->angleDelta() / 8).isNull() )
    {
        //qDebug() << event->angleDelta().x() << event->angleDelta().y();
        if ( event->angleDelta().y() > 0 )
        {
            if ( event->modifiers() == Qt::ControlModifier )
            {
                zoomInSlot();
            }
            else
            {
                upPageSlot();
            }

        }
        else
        {
            if ( event->modifiers() == Qt::ControlModifier )
            {
                zoomOutSlot();
            }
            else
            {
                downPageSlot();
            }
        }
    }

    event->accept();
}

void PDF_Widget::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void PDF_Widget::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void PDF_Widget::resizeEvent(QResizeEvent *event)
{
    if ( DocumentWidget != nullptr )
    {
        scaleToFit();
    }

    event->ignore();
}

void PDF_Widget::scaleToFit(void)
{
    /* Make sure that the document is completely visible */
    double x_factor = 0.97 * (double(width()) / physicalDpiX()) / (DocumentWidget->imageSize().width() / 72.0);
    double y_factor = 0.97 * (double(height()) / physicalDpiY()) / (DocumentWidget->imageSize().height() / 72.0);

    DocumentWidget->scalePage(x_factor < y_factor ? x_factor : y_factor);

    ScaleIndex = calcScaleIndexFromScaleFactor(DocumentWidget->scale());
}

int PDF_Widget::calcScaleIndexFromScaleFactor(double scale_factor)
{
    int scale_index = ScaleFactors.size()-1;

    for ( int i=0; i<ScaleFactors.size(); ++i )
    {
        if ( ScaleFactors[i] >= scale_factor)
        {
            scale_index = i;
            break;
        }
    }

    return scale_index;
}
