#ifndef PANELBASE_H
#define PANELBASE_H

#include <QWidget>
#include <QLabel>
#include <QColor>

class PanelBase : public QLabel
{
    Q_OBJECT

public:
    PanelBase(int size, QWidget *parent = nullptr);

    /**
     * @brief 设置背景颜色
     * @param color 背景颜色的字符串
     */
    void setColor(QString color);

protected:
    /**
     * @brief 重载paintEvent函数
     * @details 实现在控件上绘制一个圆形或方形
     * @param event 传入的绘图事件
     */
    void paintEvent(QPaintEvent *event);

private:
    /**
     * @brief 绘制所用的颜色
     */
    QColor m_color;

    /**
     * @brief 控件的最大尺寸
     */
    int m_size;
};

#endif // PANELBASE_H
