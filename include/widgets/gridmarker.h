#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QWidget>
#include <QLabel>

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

    int m_indent;  // 由于drawEllipse时边缘也有一个像素，所以画圆时需要向内缩小至少一个像素，m_indent就是控制缩进的距离

    void paintEvent(QPaintEvent *event);
};

#endif // TESTWIDGET_H
