#include "HtmlWidget.h"

#include <QVBoxLayout>
#include <QtWebEngineWidgets/QtWebEngineWidgets>

HtmlWidget::HtmlWidget(QWidget *parent) :
    QWidget(parent)
{
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    WebView = new QWebEngineView(this);
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(WebView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
}

void HtmlWidget::showUrl(QUrl url)
{
    if ( url.isEmpty() )
        return;

    WebView->history()->clear();
    WebView->setZoomFactor(1);

    //QDesktopServices::openUrl(url);
    WebView->load(url);
}

void HtmlWidget::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
}

void HtmlWidget::wheelEvent(QWheelEvent *event)
{
    if ( ! (event->angleDelta() / 8).isNull() )
    {
        //qDebug() << event->angleDelta().x() << event->angleDelta().y();
        if ( event->angleDelta().y() > 0 )
        {
            if ( event->modifiers() == Qt::ControlModifier )
            {
                WebView->setZoomFactor(WebView->zoomFactor()*1.1);
            }
            else
            {
            }

        }
        else
        {
            if ( event->modifiers() == Qt::ControlModifier )
            {
                WebView->setZoomFactor(WebView->zoomFactor()*0.9);
            }
            else
            {
            }
        }
    }

    event->accept();
}

void HtmlWidget::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void HtmlWidget::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}
