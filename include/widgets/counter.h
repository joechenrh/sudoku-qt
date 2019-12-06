/**
 * @file counter.h
 * @brief Contains widget used to show remain count of certain digit.
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef CIRCLEWIDGET_H
#define CIRCLEWIDGET_H

#include <QLabel>
#include <QGraphicsOpacityEffect>


/**
 * @brief The Counter class
 */
class Counter : public QWidget
{
    Q_OBJECT

public:
    Counter(int num, int size, QWidget *parent = nullptr);

    /**
     * @brief setCount 直接调整剩余数量，主要会在重置棋盘时用到
     * @param value 剩余数量
     */
    void setCount(int value);

    /**
     * @brief minus 减少剩余数量
     * @param value 减少的值
     */
    void minus(int value = 1);

    /**
     * @brief plus 增加剩余数量
     * @param value 增加的值
     */
    void plus(int value = 1);

protected:
    /**
     * @brief enterEvent 鼠标进入触发的事件
     * @param e 触发的事件
     */
    void enterEvent(QEvent *e = nullptr);

    /**
     * @brief leaveEvent 鼠标离开触发的事件
     * @param e 触发的事件
     */
    void leaveEvent(QEvent *e = nullptr);


signals:
    /**
     * @brief hovered 鼠标进入时触发的信号
     */
    void hovered();

    /**
     * @brief leave 鼠标离开时触发的信号
     */
    void leave();

private:
    /**
     * @brief updateOpacity 更新控件的透明度
     */
    void updateOpacity();

    /**
     * @brief m_count 剩余数量
     */
    int m_count;

    /**
     * @brief m_cntLabel 显示当前监控数字剩余数量的qlabel
     */
    QLabel *m_cntLabel;

    /**
     * @brief m_numLabel 显示当前监控数字的qlabel
     */
    QLabel *m_numLabel;

    /**
     * @brief m_cntStyle 控制cntLabel的stylesheet，
     */
    QString m_cntStyle;

    /**
     * @brief m_cntOpacity 控制cntLabel的透明度，当剩余数量小于等于0时，控件透明度会降低
     */
    QGraphicsOpacityEffect *m_cntOpacity;
};

#endif // CIRCLEWIDGET_H
