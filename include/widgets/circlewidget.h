#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QString>

class Counter : public QWidget
{
    Q_OBJECT

public:
    Counter(int num, int size, QWidget *parent = NULL);

    void setValue(int value);

    void minus();

    void plus();

    void clear();

protected:
    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

    void updateOpacity();

signals:

    void hovered();

    void leave();

private:
    int m_count;

    QLabel *m_cntLabel;

    QLabel *m_numLabel;

    QString m_baseStyle;

    int m_sr;

    int m_lr;
};

#endif // CIRCLEWIDGET_H
