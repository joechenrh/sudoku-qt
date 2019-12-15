/**
 * @file counter.h
 * @brief Contains widget used to show remain count of certain digit.
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef COUNTER_H
#define COUNTER_H

#include <QLabel>
#include <QJsonObject>
#include <QGraphicsOpacityEffect>


/**
 * @brief The Counter class
 */
class Counter : public QWidget
{
    Q_OBJECT

    struct CounterStyle
    {
        QString cnt_color_unhovered;      // cntLabel的背景色
        QString cnt_color_hovered;        // cntLabel覆盖时的背景色
        QString cnt_font_color_unhovered; // cntLabel字体颜色
        QString cnt_font_color_hovered;   // cntLabel覆盖时的字体颜色
    };

public:
    Counter(int num, int size, QWidget *parent = nullptr);

    /**
     * @brief setCount 直接调整剩余数量，主要会在重置棋盘时用到
     * @param value 剩余数量
     */
    void setCount(int value);

    /**
     * @brief 调整剩余数量
     * @param value 变化的值
     */
    void modify(int value);

    /**
     * @brief 设置配色
     * @param json 颜色字典
     */
    void setColorStyle(QJsonObject json);

protected:
    /**
     * @brief enterEvent 鼠标进入触发的事件
     * @param e 触发的事件
     */
    void enterEvent(QEvent*);

    /**
     * @brief leaveEvent 鼠标离开触发的事件
     * @param e 触发的事件
     */
    void leaveEvent(QEvent*);


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
     * @brief m_cntOpacity 控制cntLabel的透明度，当剩余数量小于等于0时，控件透明度会降低
     */
    QGraphicsOpacityEffect *m_cntOpacity;

    CounterStyle m_style;
};

#endif // COUNTER_H
