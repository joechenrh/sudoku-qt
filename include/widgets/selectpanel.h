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
     * @return 打开结果
     */
    void show();

    /**
     * @brief 关闭控件，同时触发动画效果
     * @return 关闭结果
     */
    void hide();

    /**
     * @brief setSelected 设置初始值
     * @param list 当前格子已选择的值
     */
    void setSelected(QList<int> list);

private:
    /**
     * @brief 绘制控件的掩膜
     */
    void drawMask();

    /**
     * @brief 记录多选项的数组，暂未使用
     */
    QVector<int> m_selected;

    /**
     * @brief 九宫格的九个按钮
     */
    QVector<HoverButton*> m_buttons;

    /**
     * @brief 用于触发QPropertyAnimation，动态绘制mask的的控件，无其他作用
     */
    QLabel *m_dummylabel;

    /**
     * @brief 控制控件显示和关闭的动画效果
     */
    QPropertyAnimation *m_animation;

signals:
    /**
     * @brief 选择完成时触发的信号
     * @param 被选中的数字
     */
    void finish(int);
};

#endif // SELECTPANEL_H
