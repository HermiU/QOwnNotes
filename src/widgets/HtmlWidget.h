#ifndef HTML_WIDGET_H
#define HTML_WIDGET_H

#include <QWidget>

class QWebEngineView;

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
    QWebEngineView* WebView;
};

#endif
