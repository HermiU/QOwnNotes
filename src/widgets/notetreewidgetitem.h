#ifndef NOTETREEWIDGETITEM_H
#define NOTETREEWIDGETITEM_H

#include <entities/note.h>

#include <QWidget>

namespace Ui {
class NoteTreeWidgetItem;
}

class NoteTreeWidgetItem : public QWidget {
    Q_OBJECT

   public:
    explicit NoteTreeWidgetItem(const Note &note, QWidget *parent = 0);
    void updateUserInterface(const Note &note) const;
    ~NoteTreeWidgetItem();

   private:
    Ui::NoteTreeWidgetItem *ui;
};

#endif    // NOTETREEWIDGETITEM_H
