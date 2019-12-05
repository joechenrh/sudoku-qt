#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <qDialog>
#include <QVector>
#include <QLabel>
#include <QMutex>
#include <QMutexLocker>

#include "hoverbutton.h"

class SelectPanel : public QDialog
{
    Q_OBJECT

public:

    SelectPanel(int size = 75, QWidget *parent = nullptr);

    static SelectPanel* instance(int size, QWidget *parent = nullptr);

    bool isOpened() const;

    int number() const;

    int exec();

    bool close();

    void start();

    void setBase(QWidget *widget);

protected:
    // 将鼠标进入事件转发到被覆盖的控件中
    void enterEvent(QEvent *e);

    // 将鼠标离开事件转发到被覆盖的控件中
    void leaveEvent(QEvent *e);

private:

    QWidget* m_base; //当前操作的单元格控件，用于发送鼠标事件

    bool opened;

    int selected;

    QLabel *m_dummylabel;

    QVector<int> m_selected;

    QVector<HoverButton*> m_buttons;

    static SelectPanel* m_instance;

    static QMutex m_mutex;

signals:
    void finish(int selected);
};

#endif // SELECTPANEL_H
