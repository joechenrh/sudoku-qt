#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>

#include "hoverbutton.h"
#include "selectpanel.h"
#include "gridwidget.h"
#include "counter.h"

struct Op;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

    void highlight(int num, int active);

private:
    Ui::MainWindow *ui;

    void smartAssistOn(int r, int c);

    void smartAssistOff(int r, int c);

    void changeGrid(int r, int c);

    void clearGrid(int r, int c);

    void changeNumber(int r, int c, int previous, int selected);

    void clearAll();

    void solve();

    void loadRandomPuzzle();

    void undo();

    void redo();

    QVector<QVector<QSet<QPair<int, int>>>> m_controlRanges;  // 储存每个数组单元格的影响范围

    QVector<QVector<GridWidget*>> m_grids;                    // 储存数组单元格的二维数组

    QVector<Counter*> m_counters;                             // 储存计数控件的数组

    SelectPanel *m_panel;                                     // 修改数值的面板

    QStack<Op> m_ops;                                         // 操作栈，用于记录操作

    QStack<Op> m_ops2;                                        // 操作栈，用于记录回退的操作

    QPushButton *m_undoButton;                                // 回退按钮

    QPushButton *m_redoButton;                                // 重做按钮
};

#endif // MAINWINDOW_H
