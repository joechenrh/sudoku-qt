/**
 * @file gridmarker.h
 * @brief Contains widget used to highlight mouse enter event.
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QLabel>
#include <QPainter>
#include <QPropertyAnimation>

/**
 * @brief The GridMarker class 鼠标进入九宫格时显示的圆形效果由这个控件绘制
 * @details 通过变化geometry来达到放大缩小的效果,
 */
class GridMarker : public QLabel
{
    Q_OBJECT
public:
    /**
     * @brief GridMarker 构造函数
     * @param size 控件的大小
     * @param parent 父控件
     */
    explicit GridMarker(int size, QWidget *parent = nullptr);

    /**
     * @brief 显示圆形
     */
    void hide();

    /**
     * @brief 隐藏圆形
     */
    void reveal();

private:
    /**
     * @brief 控件的宽度和高度，控件为正方形
     */
    int m_size;

    /**
     * @brief 控制绘制圆形时的缩进
     * @details 由于需要绘制一层阴影，因此实际圆形的半径需要比控件大小的一半小一点
     */
    int m_indent;

    /**
     * @brief 控制放大缩小的动画
     */
    QPropertyAnimation *m_scaleAnimation;

    /**
     * @brief 控制透明度的动画
     */
    QPropertyAnimation *m_opacityAnimation;

protected:
    /**
     * @brief 重载paintEvent函数
     * @details 实现在控件上绘制一个圆形和一层阴影
     * @param event 传入的绘图事件
     */
    void paintEvent(QPaintEvent *event);
};

#endif // TESTWIDGET_H
