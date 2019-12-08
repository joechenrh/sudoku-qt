/**
 * @file selectpanel.h
 * @brief Contains select panel widget for choose number
 * @author Joe chen <joechenrh@gmail.com>
 */


#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <QDialog>
#include <QVector>
#include <QLabel>
#include <QMutex>
#include <QList>
#include <QMutexLocker>

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
     * @brief instance 实现单例模式，仅在第一次创建实例
     * @param size 控件的大小
     * @param parent 控件的父控件
     * @return 单例
     */
    static SelectPanel* instance(int size, QWidget *parent = nullptr);

    /**
     * @brief 返回当前对话框的状态
     * @return 当前对话框是否打开
     */
    bool isOpened() const;

    /**
     * @brief 打开控件，同时触发动画效果
     * @return 打开结果
     */
    int exec();

    /**
     * @brief 关闭控件，同时触发动画效果
     * @return 关闭结果
     */
    bool close();

    /**
     * @brief 设置触发了exec()的控件，会将鼠标事件传递给此控件
     * @param widget 触发的控件
     */
    void setBase(QWidget *widget, int row, int col);

    /**
     * @brief setSelected 设置初始值
     * @param list 当前格子已选择的值
     */
    void setSelected(QList<int> list);

protected:
    /**
     * @brief 鼠标进入事件，会将事件转发到被覆盖的控件中
     * @param e 鼠标事件
     */
    void enterEvent(QEvent *e);

    /**
     * @brief 鼠标移出事件，会将事件转发到被覆盖的控件中
     * @param e 鼠标事件
     */
    void leaveEvent(QEvent *e);

private:
    /**
     * @brief 触发单元格所处行
     */
    int m_row;

    /**
     * @brief 触发单元格所处列
     */
    int m_col;

    /**
     * @brief 绘制控件的掩膜
     */
    void drawMask();

    /**
     * @brief 触发了exec()的控件
     * @see setBase
     */
    QWidget* m_base;

    /**
     * @brief 记录实例是否已经被打开
     */
    bool opened;

    /**
     * @brief 记录当前选择的数字
     */
    int selected;

    /**
     * @brief 用于触发QPropertyAnimation，动态绘制mask的的控件，无其他作用
     */
    QLabel *m_dummylabel;

    /**
     * @brief 记录多选项的数组，暂未使用
     */
    QVector<int> m_selected;

    /**
     * @brief 九宫格的九个按钮
     */
    QVector<HoverButton*> m_buttons;

    /**
     * @brief 对话框单例
     */
    static SelectPanel* m_instance;

    /**
     * @brief 互斥量
     */
    static QMutex m_mutex;

    /**
     * @brief 控制控件显示和关闭的动画效果
     */
    QPropertyAnimation *m_animation;

signals:
    /**
     * @brief 选择完成时触发的信号
     * @param 被选中的数字
     */
    void finish(QList<int>);
};

#endif // SELECTPANEL_H
