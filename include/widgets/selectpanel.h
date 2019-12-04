#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <qDialog>
#include <QVector>

#include "hoverbutton.h"

class SelectPanel : public QDialog
{
    Q_OBJECT

public:

    SelectPanel(int size = 75, QWidget *parent = NULL);

    int number() const;

    int exec();

    bool close();

    void start();

private:

    int selected;

    QVector<HoverButton*> m_buttons;

};

#endif // SELECTPANEL_H
