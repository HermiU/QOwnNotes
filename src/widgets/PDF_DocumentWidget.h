#ifndef PDF_DOCUMENT_WIDGET_H
#define PDF_DOCUMENT_WIDGET_H

#include <QLabel>
#include <QRectF>
#include "poppler-qt5.h"

class PDF_DocumentWidget : public QLabel
{
    Q_OBJECT
public:
    PDF_DocumentWidget(QWidget *parent = nullptr);
    ~PDF_DocumentWidget();

    qreal scale() const;
    int page() const;
    int pages() const;
    QSize imageSize() const;

    bool setDocument(const QString filename);
    void setPage(const int new_page);
    void scalePage(const qreal scale);

private:
    static constexpr int INVALID_PAGE = 0;

    void showPage(const int page);

    Poppler::Document* PDF_Document;

    int   CurrentPage;
    qreal ScaleFactor;

    Poppler::Page::Rotation Rotation;
};

#endif
