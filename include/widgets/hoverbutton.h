/**
 * @file hoverbutton.h
 * @brief Contains button widget
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef HOVERBUTTON_H
#define HOVERBUTTON_H

#include <QPushButton>
#include <QMouseEvent>
#include <QPropertyAnimation>

/**
 * @brief The HoverButton class 按钮控件
 * @details 相对于基础的鼠标控件，增加了右键事件和淡入淡出效果
 * 主要是为了GridWidget服务
 */
class HoverButton : public QPushButton
{
    Q_OBJECT

public:
    HoverButton(QWidget *parent = NULL);

    /**
     * @brief 隐藏当前控件
     */
    void hide();

    /**
     * @brief 显示当前控件
     */
    void reveal();

protected:
    /**
     * @brief 鼠标移入触发的事件
     * @param e 鼠标事件
     */
    void enterEvent(QEvent *);

    /**
     * @brief 鼠标移出触发的事件
     * @param e 鼠标事件
     */
    void leaveEvent(QEvent *);

private slots:
    /**
     * @brief 鼠标点击事件
     * @param e 鼠标事件
     * @details 重写以支持右键点击事件
     */
    void mousePressEvent(QMouseEvent *e);

signals:
    /**
     * @brief 鼠标进入时触发的信号
     */
    void hovered();

    /**
     * @brief 鼠标移出时触发的信号
     */
    void leaved();

    /**
     * @brief 鼠标右击触发的信号
     */
    void rightClicked();

private:
    /**
     * @brief 淡出动画
     */
    QPropertyAnimation *m_animation;

};

#endif // HOVERBUTTON_H
