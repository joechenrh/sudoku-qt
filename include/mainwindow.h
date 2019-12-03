#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QVector>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QStack>

#include "hoverbutton.h"
#include "selectpanel.h"
#include "gridwidget.h"
#include "circlewidget.h"

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

    void changeBackground(int r, int c);

    void recoverBackground(int r, int c);

    void changeNumber(int r, int c);

    void clearAll();

    void clearGrid(int r, int c);

    void solve();

    void loadRandomPuzzle();

    QVector<QVector<QSet<QPair<int, int>>>> m_controlRanges;

    QVector<QVector<GridWidget*>> m_grids;

    QVector<Counter*> m_counters;

    SelectPanel *m_panel;

    QStack<Op> m_ops;  // add Redo/Undo
};

#endif // MAINWINDOW_H
