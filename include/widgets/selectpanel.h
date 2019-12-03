#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <qDialog>
#include <QTimer>
#include <QVector>
#include <QPaintEvent>

#include "hoverbutton.h"

class SelectPanel : public QDialog
{
    Q_OBJECT

public:

    SelectPanel(int size = 75, QWidget *parent = NULL);

    int number();

    int exec();

    bool close();

    void start();

private:

    int selected;

    QVector<HoverButton*> m_buttons;

};

#endif // SELECTPANEL_H
