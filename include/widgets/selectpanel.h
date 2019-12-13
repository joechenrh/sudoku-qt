/**
 * @file selectpanel.h
 * @brief Contains select panel widget for choose number
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <QVector>
#include <QLabel>
#include <QList>

#include "hoverbutton.h"

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


/**
 * @brief The SelectPanel class 修改数字的面板
 * @details 使用SelectPanel::instance()获取单例
 * 在打开关闭时会有淡入淡出效果
 */
class SelectPanel : public QWidget
{
    Q_OBJECT

public:
    SelectPanel(int size = 75, QWidget *parent = nullptr);

    /**
     * @brief 打开控件，同时触发动画效果
     * @param x 打开时所在的x坐标
     * @param y 打开时所在的y坐标
     * @details 如果在调用打开时，控件正在关闭，则会等待控件关闭后，再打开
     */
    bool show(int x, int y);

    /**
     * @brief 关闭控件，同时触发动画效果
     * @return 是否成功关闭
     * @details 如果在调用隐藏时，控件正在打开，则不会进行任何操作，并返回失败
     */
    bool hide();

    /**
     * @brief isVisible
     * @return
     */
    bool isVisible() const;

    /**
     * @brief setSelected 设置初始值
     * @param list 当前格子已选择的值
     */
    void setSelected(QList<int> list);

private:
    void initialize();


    /**
     * @brief 记录多选的数据，暂未使用
     */
    QVector<int> m_selected;

    QPropertyAnimation *m_showAnimation;

    QPropertyAnimation *m_hideAnimation;

    /**
     * @brief 九宫格的九个按钮
     */
    QVector<HoverButton*> m_buttons;

    /**
     * @brief 面板的背景
     */
    PanelBase *m_background;

    /**
     * @brief 面板的前景，包含了九个按钮
     */
    QWidget *m_container;

signals:
    /**
     * @brief 选择完成时触发的信号
     * @param 被选中的数字
     */
    void finish(int);
};

#endif // SELECTPANEL_H
