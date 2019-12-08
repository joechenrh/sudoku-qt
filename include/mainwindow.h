/**
 * @file mainwindow.h
 * @brief Contains mainwindow of the game
 * @author Joe chen <joechenrh@gmail.com>
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

#include "hoverbutton.h"
#include "selectpanel.h"
#include "gridwidget.h"
#include "counter.h"

/**
 * @brief The Op struct
 */
struct Op
{
    int row;    // 操作的行数
    int col;    // 操作的列数
    int before; // 更改前的值
    int after;  // 更改后的值
    Op(int _row = 0, int _col = 0, int _before = 0, int _after = 0)
        : row(_row), col(_col), before(_before), after(_after) {}
};


namespace Ui {
class MainWindow;
}

/**
 * @brief The MainWindow class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    /**
     * @brief highlight 高亮/取消高亮选中的数字
     * @param num 所选数字
     * @param active 开启或关闭
     */
    void highlight(int num, int active);

private slots:
    /**
     * @brief 接收结果
     * @param result 长度为三的数组，分别是改变值，行，列
     */
    void receiveResult(QList<int> result);

    /**
     * @brief 求解当前数独
     */
    void solve();

    /**
     * @brief 随机加载数独
     */
    void loadRandomPuzzle();

    /**
     * @brief 撤回上一步操作
     */
    void undo();

    /**
     * @brief 重做上一步操作
     */
    void redo();

    /**
     * @brief 清除九宫格，不包括谜面
     */
    void clearAll();

private:
    Ui::MainWindow *ui;

    /**
     * @brief 智能提示当前格子的影响范围
     * @param r 所选行
     * @param c 所选列
     */
    void smartAssistOn(int r, int c);

    /**
     * @brief 关闭智能提示
     * @param r 所选行
     * @param c 所选列
     * @see smartAssistOn
     */
    void smartAssistOff(int r, int c);

    /**
     * @brief 清除某个单元格的值
     * @param r 所选行
     * @param c 所选列
     */
    void clearGrid(int r, int c);

    /**
     * @brief changeNumber 调整某个单元格的值
     * @param r 所选行
     * @param c 所选列
     * @param previous 修改之前的值
     * @param selected 修改之后的值
     */
    void changeNumber(int r, int c, int previous, int selected);


    int m_sr;

    int m_sc;

    /**
     * @brief 储存每个数组单元格的影响范围
     */
    QVector<QVector<QSet<QPair<int, int>>>> m_controlRanges;

    /**
     * @brief 储存单元格控件的二维数组
     */
    QVector<QVector<GridWidget*>> m_grids;

    /**
     * @brief 储存计数控件的数组
     */
    QVector<Counter*> m_counters;

    /**
     * @brief 修改数值的面板
     */
    SelectPanel *m_panel;

    /**
     * @brief 操作栈，用于记录操作
     */
    QStack<Op> m_undoOps;

    /**
     * @brief 操作栈，用于记录回退的操作
     */
    QStack<Op> m_redoOps;

    /**
     * @brief 回退按钮
     */
    QPushButton *m_undoButton;

    /**
     * @brief 重做按钮
     */
    QPushButton *m_redoButton;
};

#endif // MAINWINDOW_H
