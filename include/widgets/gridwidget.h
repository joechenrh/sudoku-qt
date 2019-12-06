/**
 * @file gridwidget.h
 * @brief Contains main component of sudoku
 * @author Joe chen <joechenrh@gmail.com>
 */

#ifndef GRIDWIDGET_H
#define GRIDWIDGET_H

#include <QLabel>

#include "hoverbutton.h"
#include "gridmarker.h"
#include "selectpanel.h"

/**
 * @brief The GridWidget class
 * @details 该控件组成了数组的主要窗体，主要有以下几个功能：
 * 1. 选择和清除当前格的数值
 * 2. 记录和当前格冲突的单元格数量，在存在冲突时，会绘制一个圆环加以提示
 * 3. 鼠标进入时会出现一个标记，同时会高亮当前单元格
 */
class GridWidget : public QWidget
{
    Q_OBJECT
public:

    explicit GridWidget(int row, int col, int size = 75, QWidget *parent = nullptr);

    /**
     * @brief setEnabled 设置当前控件是否可操作
     * @param flag 是否可操作
     * @details 谜面所在的单元格是不能编辑的，也不会触发任何鼠标事件
     */
    void setEnabled(bool flag);

    /**
     * @brief 返回当前控件是否可操作
     * @return  是否可操作
     * @see setEnabled
     */
    bool isEnabled() const;

    /**
     * @brief 设置该格的值
     * @param value 该格选中的值，如没有值（例如清空操作），输入0
     */
    void setValue(int value);

    /**
     * @brief 返回选中的值
     * @return 该格选中的值，如没选中，返回0
     */
    int value() const;

    /**
     * @brief hideButton
     */
    void hideButton();

    /**
     * @brief revealButton
     */
    void revealButton();

    /**
     * @brief 添加冲突数
     * @param num 新增的冲突数
     */
    void addConflict(int num);

    /**
     * @brief 减少冲突数
     * @param num 减少的冲突数
     * @
     */
    void removeConflict(int num);

    /**
     * @brief 清空冲突数
     */
    void clearConflict();

protected:
    /**
     * @brief 鼠标移入触发的事件
     * @param e 鼠标事件
     */
    void enterEvent(QEvent *e);

    /**
     * @brief 鼠标移出触发的事件
     * @param 鼠标事件
     */
    void leaveEvent(QEvent *e);

signals:
    /**
     * @brief 鼠标移入触发的信号
     */
    void hovered();

    /**
     * @brief 鼠标移出触发的信号
     */
    void leaved();

    /**
     * @brief 顶层按钮被左击触发的信号
     */
    void clicked();

    /**
     * @brief 顶层按钮被右击触发的信号
     */
    void rightClicked();

private slots:
    /**
     * @brief 顶层按钮被左击时的slot
     */
    void buttonClicked();

    /**
     * @brief 顶层按钮被右击时的slot
     */
    void buttonRightClicked();

private:
    SelectPanel *m_panel;

    /**
     * @brief 单元格所处行
     */
    int m_row;

    /**
     * @brief 单元格所处列
     */
    int m_col;

    /**
     * @brief 最顶层的按钮，用于显示数值和触发单击事件
     */
    HoverButton *m_button;

    /**
     * @brief 鼠标覆盖时的标记
     */
    GridMarker *m_marker;

    /**
     * @brief 底层的前景
     */
    HoverButton *m_foreground;

    /**
     * @brief 底层的背景
     */
    QLabel *m_background;

    /**
     * @brief 顶层按钮的stylesheet
     */
    QString m_buttonStyle;

    /**
     * @brief 当前单元格是否可编辑
     * @see setEnabled
     */
    bool m_enabled;

    /**
     * @brief 当前单元格的值
     */
    int m_value;

    /**
     * @brief 冲突的单元格数量
     * @details 当前行，列，九宫格内，值和自身值一样的单元格的数量
     */
    int m_numConflict;

    /**
     * @brief 边框的宽度，只可能为0或3
     * @details 单元格发生冲突时显示的白圈
     * 就是通过控件的border属性来绘制的
     */
    int m_borderRadius;

    /**
     * @brief 边框的颜色
     * @see m_borderRadius
     */
    QString m_borderColor;

    /**
     * @brief 字体的颜色
     */
    QString m_fontColor;
};

#endif // GRIDWIDGET_H
