#ifndef PDF_WIDGET_H
#define PDF_WIDGET_H

#include <QVector>
#include <QWidget>

class QScrollArea;
class PDF_DocumentWidget;

class PDF_Widget : public QWidget
{
    Q_OBJECT
public:
    PDF_Widget(QWidget *parent = nullptr);

    void showPDF_File(QString filename);

private slots:
    void scaleDocumentSlot(int index);
    void zoomInSlot(void);
    void zoomOutSlot(void);

    void setPageSlot(int page);
    void upPageSlot(void);
    void downPageSlot(void);

protected:
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void resizeEvent(QResizeEvent* event);

private:
    int calcScaleIndexFromScaleFactor(double scale_factor);
    void scaleToFit(void);

    PDF_DocumentWidget* DocumentWidget;

    QScrollArea* ScrollArea;

    QVector<qreal> ScaleFactors;
    int            ScaleIndex;

    int SelectedPage;
};

#endif
