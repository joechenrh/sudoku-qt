#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTime>
#include <QDebug>
#include <QFontDatabase>
#include <QGraphicsDropShadowEffect>

#include "sudokusolver.h"

#define margin 10              // 四周的边缘宽度
#define gridSize 75            // 格子的大小
#define halfSize gridSize / 2  // 半个格子的大小，也是按钮的高度
#define spacing 5              // 九宫格之间的间隔

QPushButton* createButton(QString text)
{
    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont buttonFont = QFont(strList.at(0), 12);
    buttonFont.setBold(true);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setOffset(1, 1);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(2);

    QPushButton *button = new QPushButton(text);
    button->setFont(buttonFont);
    button->setGraphicsEffect(shadow_effect);
    button->setStyleSheet(QString("QWidget{border-radius:%1px;background-color:#FFFFFF;color:#5F5F5F;}"
                                  "QPushButton:hover{background-color:rgb(236,236,236);}"
                                  "QPushButton:pressed{background-color:rgb(222,222,222);}").arg(gridSize / 2));
    return button;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow), m_sr(0), m_sc(0)
{
    // 窗口设置
    QColor color = QColor("#DDE2E5");
    QPalette p = this->palette();
    p.setColor(QPalette::Window,color);
    this->setPalette(p);

    /*********************************************/

    m_grids.resize(9);
    m_controlRanges.resize(9);
    for(int r = 0; r < 9; r++)
    {
        m_grids[r].resize(9);
        m_controlRanges[r].resize(9);
        for (int c = 0; c < 9; c++)
        {
            // initialize control range, doesn't include self
            for (int i = 0; i < 9; i++)
            {
                m_controlRanges[r][c].insert(qMakePair(r, i));
                m_controlRanges[r][c].insert(qMakePair(i, c));
            }
            for (int i = r / 3 * 3; i < r / 3 * 3 + 3; i++)
            {
                for (int j = c / 3 * 3; j < c / 3 * 3 + 3; j++)
                {
                   m_controlRanges[r][c].insert(qMakePair(i, j));
                }
            }
            m_controlRanges[r][c].erase(m_controlRanges[r][c].find(qMakePair(r, c)));

            GridWidget *grid = new GridWidget(r, c, gridSize);
            grid->setParent(this);
            grid->move(margin + c * gridSize + c / 3 * spacing, margin + r * gridSize + r / 3 * spacing);
            m_grids[r][c] = grid;

            connect(grid, &GridWidget::hovered,      [=]()
            {
                if (!grid->isEnabled() || m_panel->isVisible())
                {
                    return;
                }
                smartAssistOn(r, c);
                grid->enter();
            });

            connect(grid, &GridWidget::leaved,       [=]()
            {
                if (!grid->isEnabled() || m_panel->isVisible())
                {
                    return;
                }
                smartAssistOff(r, c);
                grid->leave();
            });

            connect(grid, &GridWidget::rightClicked, [=]()
            {
                if (m_panel->isVisible())
                {
                    // 尝试关闭
                    if( m_panel->hide())
                    {
                        m_grids[m_sr][m_sc]->leave();
                        smartAssistOff(m_sr, m_sc);
                    }
                    return;
                }
                clearGrid(r, c);
            });

            connect(grid, &GridWidget::clicked,      [=](){
                if (m_panel->isVisible())
                {
                    // 尝试关闭
                    if( m_panel->hide())
                    {
                        m_grids[m_sr][m_sc]->leave();
                        smartAssistOff(m_sr, m_sc);
                    }
                    smartAssistOn(r, c);
                    grid->enter();
                }
                m_sr = r;
                m_sc = c;
                m_panel->show(grid->geometry().x(), grid->geometry().y());
            });
        }
    }

    QPushButton *loadButton = createButton("load");
    loadButton->setParent(this);
    loadButton->move(margin + (gridSize * 3 + spacing) * 0, margin + gridSize * 9 + halfSize);
    loadButton->setFixedSize(gridSize * 3, gridSize);

    QPushButton *solveButton = createButton("solve");
    solveButton->setParent(this);
    solveButton->move(margin + (gridSize * 3 + spacing) * 1, margin + gridSize * 9 + halfSize);
    solveButton->setFixedSize(gridSize * 3, gridSize);

    QPushButton *clearButton = createButton("clear");
    clearButton->setParent(this);
    clearButton->move(margin + (gridSize * 3 + spacing) * 2, margin + gridSize * 9 + halfSize);
    clearButton->setFixedSize(gridSize * 3, gridSize);

    int nIndex = QFontDatabase::addApplicationFont(":/sudoku/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    m_undoButton = new QPushButton("<");
    m_undoButton->setParent(this);
    m_undoButton->setFont(QFont(strList.at(0), 12));
    m_undoButton->move(margin + gridSize * 9 + halfSize, margin + gridSize * 9 + halfSize);
    m_undoButton->setFixedSize(halfSize, gridSize);
    m_undoButton->setStyleSheet(QString("QWidget{background-color:#FFFFFF;color:#5F5F5F;"
                                        "border-top-left-radius:%1px;border-bottom-left-radius:%1px;}"
                                        "QPushButton:hover{background-color:rgb(236, 236, 236);}"
                                        "QPushButton:pressed{background-color:rgb(222, 222, 222);}"
                                        "QPushButton:!enabled{background-color:rgb(200, 200, 200);}")
                                        .arg(gridSize / 5));

    m_redoButton = new QPushButton(">");
    m_redoButton->setParent(this);
    m_redoButton->setFont(QFont(strList.at(0), 12));
    m_redoButton->move(margin + gridSize * 10, margin + gridSize * 9 + halfSize);
    m_redoButton->setFixedSize(halfSize, gridSize);
    m_redoButton->setStyleSheet(QString("QWidget{background-color:#FFFFFF;color:#5F5F5F;"
                                        "border-top-right-radius:%1px;border-bottom-right-radius:%1px;}"
                                        "QPushButton:hover{background-color:rgb(236, 236, 236);}"
                                        "QPushButton:pressed{background-color:rgb(222, 222, 222);}"
                                        "QPushButton:!enabled{background-color:rgb(200, 200, 200);}")
                                        .arg(gridSize / 5));

    m_panel = new SelectPanel(gridSize);
    m_panel->setParent(this);

    connect(loadButton,   SIGNAL(clicked()),   this, SLOT(loadRandomPuzzle()));
    connect(clearButton,  SIGNAL(clicked()),   this, SLOT(clearAll()));
    connect(solveButton,  SIGNAL(clicked()),   this, SLOT(solve()));
    connect(m_undoButton, SIGNAL(clicked()),   this, SLOT(undo()));
    connect(m_redoButton, SIGNAL(clicked()),   this, SLOT(redo()));
    connect(m_panel,      SIGNAL(finish(int)), this, SLOT(receiveResult(int)));

    int space = std::min(spacing / 10, 2);
    for (int i = 0; i < 9; i++)
    {
       Counter *counter = new Counter(i + 1, gridSize, this);
       counter->move(margin + gridSize * 9 + halfSize, margin + space + i * (space + gridSize));
       m_counters.push_back(counter);
       connect(counter, &Counter::hovered, [=]() {highlight(i + 1, true);});
       connect(counter, &Counter::leave,   [=]() {highlight(i + 1, false);});
    }

    loadRandomPuzzle();

    int minSize = gridSize * 10 + halfSize + 2 * margin;
    this->setMinimumSize(minSize, minSize);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_panel;
}


void MainWindow::receiveResult(int selected)
{
    int previous = m_grids[m_sr][m_sc]->value();
    // 选择和之前相同表示清空当前格子
    if (previous == selected)
    {
        selected = 0;
    }

    changeNumber(m_sr, m_sc, previous, selected);

    m_undoOps.append(Op(m_sr, m_sc, previous, selected));
    m_undoButton->setEnabled(true);

    m_redoOps.clear();
    m_redoButton->setEnabled(false);
}

void MainWindow::highlight(int num, int active)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (m_grids[r][c]->value() == num)
            {
                if (active)
                {
                    m_grids[r][c]->showBackground();
                }
                else
                {
                    m_grids[r][c]->hideBackground();
                }

            }
        }
    }
}

void MainWindow::changeNumber(int r, int c, int previous, int selected)
{
    m_grids[r][c]->setValue(selected);

    int num = 0;

    // 冲突检测，将和旧值相同的格子冲突数都减去1
    if (previous > 0)
    {
        m_counters[previous - 1]->plus();
        for (auto pair : m_controlRanges[r][c])
        {
            if (m_grids[pair.first][pair.second]->value() == previous)
            {
                m_grids[pair.first][pair.second]->changeConflict(-1);
                --num;
            }
        }
    }

    // 如果新的值不为0，将和新值相同的格子冲突数加1，再计算本身的冲突数
    if (selected > 0)
    {
        m_counters[selected - 1]->minus();
        for (auto pair : m_controlRanges[r][c])
        {
            if (m_grids[pair.first][pair.second]->value() == selected)
            {
                m_grids[pair.first][pair.second]->changeConflict(1);
                ++num;
            }
        }
    }

    m_grids[r][c]->changeConflict(num);
}

void MainWindow::clearGrid(int r, int c)
{
    int previous = m_grids[r][c]->value();
    // 之前也为空就什么也不做
    if (previous == 0)
    {
        return;
    }

    changeNumber(r, c, previous, 0);

    m_undoOps.append(Op(r, c, previous, 0));
    m_undoButton->setEnabled(true);

    m_redoOps.clear();
    m_redoButton->setEnabled(false);
}

void MainWindow::clearAll()
{
    QVector<int> counts(10, 0);
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (m_grids[r][c]->isEnabled() && m_grids[r][c]->value())
            {
                ++counts[m_grids[r][c]->value()];
                m_grids[r][c]->setValue(0);
            }
            m_grids[r][c]->clearConflict();
        }
    }

    for (int i = 1; i < 10; i++)
    {
        m_counters[i - 1]->plus(counts[i]);
    }

    m_undoOps.clear();
    m_undoButton->setEnabled(false);

    m_redoOps.clear();
    m_redoButton->setEnabled(false);
}


void MainWindow::smartAssistOff(int r, int c)
{
    qDebug() << "Off";
    for (auto &pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->hideBackground();
    }
    m_grids[r][c]->hideBackground();
}

void MainWindow::smartAssistOn(int r, int c)
{
    qDebug() << "On";
    for (auto &pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->showBackground();
    }
    m_grids[r][c]->showBackground();
}


// 随机生成谜题
void MainWindow::loadRandomPuzzle()
{    
    // 统计文件数量
    QDir *dir = new QDir(":/sudoku/puzzles");
    QStringList filter;
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    int count = fileInfo->count();  //文件个数

    // 随机选取文件
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    int n = qrand() % count;
    QString fileName = QString(":/sudoku/puzzles/%1.txt").arg(n + 1);

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QVector<int> counts(10, 0);

    int val;
    QString array = file.readAll();
    QStringList rows = array.split('\n');
    for (int r = 0; r < 9; r++)
    {
        QStringList cols = rows.at(r).split(' ');
        for (int c = 0; c < 9; c++)
        {
            val = cols.at(c).toInt();
            m_grids[r][c]->setEnabled(val == 0);  // 值为0表示待填充，即可操作
            m_grids[r][c]->setValue(val);
            ++counts[val];
        }
    }

    for (int i = 1; i <= 9; i++)
    {
        m_counters[i - 1]->setCount(9 - counts[i]);
    }

    m_undoOps.clear();
    m_undoButton->setEnabled(false);
    m_redoButton->setEnabled(false);
}

// 没有结果时的处理
void MainWindow::solve()
{
    QVector<QVector<int>> puzzle(9, QVector<int>(9, 0));
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            puzzle[r][c] = m_grids[r][c]->value();
        }
    }

    SudokuSolver solver(puzzle);
    solver.Solve();

    for (int r = 0; r < 9; r++)
    {
        m_counters[r]->setCount(0);
        for (int c = 0; c < 9; c++)
        {
            m_grids[r][c]->setValue(solver.m_res[r][c]);
        }
    }
}


void MainWindow::redo()
{
    Op op = m_redoOps.pop();
    m_undoOps.push(op);
    changeNumber(op.row, op.col, op.before, op.after);

    m_undoButton->setEnabled(true);
    m_undoButton->setEnabled(m_redoOps.size() > 0);
}

void MainWindow::undo()
{
    Op op = m_undoOps.pop();
    m_redoOps.push(op);
    changeNumber(op.row, op.col, op.after, op.before);

    m_redoButton->setEnabled(true);
    m_redoButton->setEnabled(m_undoOps.size() > 0);
}
