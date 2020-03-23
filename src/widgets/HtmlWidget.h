#ifndef HTML_WIDGET_H
#define HTML_WIDGET_H

#include <QWidget>

#ifdef WITH_WEB_ENGINE
class QWebEngineView;
#else
class QWebView;
#endif

class HtmlWidget : public QWidget
{
    Q_OBJECT
public:
    HtmlWidget(QWidget *parent = nullptr);

    void showUrl(QUrl url);

protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
#ifdef WITH_WEB_ENGINE
    QWebEngineView* WebView;
#else
    QWebView* WebView;
#endif
};

#endif
