#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>

#include <QLabel>
#include <QPushButton>

#include <QPainter>
#include <QPropertyAnimation>


class GridMarker : public QLabel
{
    Q_OBJECT
public:
    explicit GridMarker(int size, QWidget *parent = nullptr);

    void hide();

    void reveal();

private:
    QPropertyAnimation *m_scaleAnimation;

    QPropertyAnimation *m_opacityAnimation;

    int m_size;

    void paintEvent(QPaintEvent *event);

    void enterEvent(QEvent *e);

    void leaveEvent(QEvent *e);

signals:
    void entered();

    void leaved();

public slots:
};

#endif // TESTWIDGET_H
