#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QTime>
#include <QDebug>
#include <QFontDatabase>

#include "sudokusolver.h"

#define margin 15              // 四周的边缘宽度
#define gridSize 81            // 格子的大小
#define halfSize gridSize / 2  // 半个格子的大小，也是按钮的高度
#define spacing 5              // 九宫格之间的间隔

QPushButton* createButton(QWidget* parent, QSize size, QString text)
{
    int nIndex = QFontDatabase::addApplicationFont(":/fonts/ARLRDBD.TTF");
    QStringList strList(QFontDatabase::applicationFontFamilies(nIndex));

    QFont buttonFont = QFont(strList.at(0), 12);
    buttonFont.setBold(true);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setOffset(1, 1);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(2);

    QPushButton *button = new QPushButton(text);
    button->setParent(parent);
    button->setFont(buttonFont);
    button->setFixedSize(size);
    button->setGraphicsEffect(shadow_effect);

    return button;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
  , m_sr(-1), m_sc(-1), m_switching(false), m_forcing(false)
{
    // 窗口设置
    QColor color = QColor("#DDE2E5");
    QPalette p = this->palette();
    p.setColor(QPalette::Window,color);
    this->setPalette(p);

    /*********************************************/

    m_grids.resize(9);
    m_counters.resize(10);
    m_numPositions.resize(10);
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

            GridWidget *grid = new GridWidget(r, c, gridSize, this);
            grid->move(margin + c * gridSize + c / 3 * spacing, margin + r * gridSize + r / 3 * spacing);
            m_grids[r][c] = grid;

            // 问题1：取消后立刻移到被取消的单元格，marker会出现，原因：off比hover里的on晚触发
            // 问题2：面板显示时，左击附近格子再立刻移入之前的格子，会出现marker | 通过添加m_switching解决
            // 问题3：点选数字后迅速移开，当前格子还会有marker | 通过添加m_forcing解决

            connect(grid, &GridWidget::hovered, [=]()
            {
                if ((!m_switching && grid->isEnabled() && !m_panel->isVisible()) || m_forcing)
                {
                    smartAssistOn(r, c);
                    grid->enter();
                }
            });

            connect(grid, &GridWidget::leaved, [=]()
            {
                if (grid->isEnabled() && !m_panel->isVisible())
                {
                    m_sr = m_sc = -1;
                    smartAssistOff(r, c);
                    grid->leave();
                }
            });

            connect(grid, &GridWidget::rightClicked, [=]()
            {
                if (m_panel->isVisible() && m_panel->hide())
                {
                    if (m_sr >= 0 && m_sc >= 0)
                    {
                        smartAssistOff(m_sr, m_sc);
                    }
                    return;
                }
                clearGrid(r, c);
            });

            connect(grid, &GridWidget::clicked, [=](){
                m_switching = true;
                if (!m_panel->isVisible() || m_panel->hide())
                {
                    m_switching = false;
                    m_panel->show(grid->geometry().x(), grid->geometry().y());
                    if (m_sr >= 0 && m_sc >= 0)
                    {
                        smartAssistOff(m_sr, m_sc);
                    }
                    grid->leave();
                    smartAssistOn(r, c);
                    m_sr = r;
                    m_sc = c;
                }
            });
        }
    }

    /***************************************/
    // 按钮
    // 所有按钮的样式，圆角等属性每个按钮自己设定
    this->setStyleSheet("MainWindow QPushButton{background-color:#FFFFFF;color:#5F5F5F;}"
                        "MainWindow QPushButton:hover{background-color:rgb(236, 236, 236);}"
                        "MainWindow QPushButton:pressed{background-color:rgb(222, 222, 222);}"
                        "MainWindow QPushButton:!enabled{background-color:rgb(200, 200, 200);}");

    // 加载按钮
    QPushButton *loadButton = createButton(this, QSize(gridSize * 3, gridSize), "Load");
    loadButton->setStyleSheet(QString("border-radius:%1px;").arg(halfSize));
    loadButton->move(margin + (gridSize * 3 + spacing) * 0, margin + gridSize * 9 + halfSize);
    connect(loadButton, SIGNAL(clicked()), this, SLOT(loadRandomPuzzle()));

    // 求解按钮
    QPushButton *solveButton = createButton(this, QSize(gridSize * 3, gridSize), "Solve");
    solveButton->setStyleSheet(QString("border-radius:%1px;").arg(halfSize));
    solveButton->move(margin + (gridSize * 3 + spacing) * 1, margin + gridSize * 9 + halfSize);
    connect(solveButton, SIGNAL(clicked()), this, SLOT(solve()));

    // 清空按钮
    QPushButton *clearButton = createButton(this, QSize(gridSize * 3, gridSize), "Clear");
    clearButton->setStyleSheet(QString("border-radius:%1px;").arg(halfSize));
    clearButton->move(margin + (gridSize * 3 + spacing) * 2, margin + gridSize * 9 + halfSize);
    connect(clearButton, SIGNAL(clicked()), this, SLOT(clearAll()));

    // 回退按钮
    m_undoButton = createButton(this, QSize(halfSize, gridSize), "<");
    m_undoButton->move(margin + gridSize * 9 + halfSize, margin + gridSize * 9 + halfSize);
    m_undoButton->setStyleSheet(QString("border-top-left-radius:%1px;border-bottom-left-radius:%1px;").arg(halfSize));
    connect(m_undoButton, SIGNAL(clicked()), this, SLOT(undo()));

    // 重做按钮
    m_redoButton = createButton(this, QSize(halfSize, gridSize), ">");
    m_redoButton->move(margin + gridSize * 10, margin + gridSize * 9 + halfSize);
    m_redoButton->setStyleSheet(QString("border-top-right-radius:%1px;border-bottom-right-radius:%1px;").arg(halfSize));
    connect(m_redoButton, SIGNAL(clicked()), this, SLOT(redo()));

    /***************************************/

    m_panel = new SelectPanel(gridSize, this);
    connect(m_panel, &SelectPanel::finish, [&](int selected)
    {
        smartAssistOff(m_sr, m_sc);
        receiveResult(selected);

        m_forcing = true;
        m_panel->hide();
        m_forcing = false;
    });

    int space = std::min(spacing / 10, 2);
    for (int num = 1; num <= 9; ++num)
    {
       Counter *counter = new Counter(num, gridSize, this);
       counter->move(margin + gridSize * 9 + halfSize, margin - gridSize + num * (space + gridSize));
       connect(counter, &Counter::hovered, [=]() {highlight(num, true);});
       connect(counter, &Counter::leave,   [=]() {highlight(num, false);});
       m_counters[num] = counter;
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
    for (auto pair : m_numPositions[num])
    {
        if (active)
        {
           m_grids[pair.first][pair.second]->showBackground();
        }
        // 取消高亮时如果在选择状态，被覆盖的块不会被取消高亮
        else if (!m_panel->isVisible() ||
                 m_controlRanges[m_sr][m_sc].find(pair) == m_controlRanges[m_sr][m_sc].end())
        {
            m_grids[pair.first][pair.second]->hideBackground();
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
        m_counters[previous]->modify(1);
        m_numPositions[previous].erase(m_numPositions[previous].find(qMakePair(r, c)));
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
        m_counters[selected]->modify(-1);
        m_numPositions[selected].insert(qMakePair(r, c));
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
    if (m_panel->isVisible())
    {
        return;
    }

    QVector<int> counts(10, 0);
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            int value = m_grids[r][c]->value();
            if (m_grids[r][c]->isEnabled() && value)
            {
                ++counts[value];
                m_grids[r][c]->setValue(0);
                m_numPositions[value].erase(m_numPositions[value].find(qMakePair(r, c)));
            }
            m_grids[r][c]->clearConflict();
        }
    }

    for (int num = 1; num <= 9; ++num)
    {
        m_counters[num]->modify(counts[num]);
    }

    m_undoOps.clear();
    m_undoButton->setEnabled(false);

    m_redoOps.clear();
    m_redoButton->setEnabled(false);
}


void MainWindow::smartAssistOff(int r, int c)
{
    for (auto &pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->hideBackground();
    }
    m_grids[r][c]->hideBackground();
}

void MainWindow::smartAssistOn(int r, int c)
{
    for (auto &pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->showBackground();
    }
    m_grids[r][c]->showBackground();
}


// 随机生成谜题
void MainWindow::loadRandomPuzzle()
{    
    if (m_panel->isVisible())
    {
        return;
    }

    // 统计文件数量
    QDir *dir = new QDir(":/puzzles");
    QStringList filter;
    QList<QFileInfo> *fileInfo = new QList<QFileInfo>(dir->entryInfoList(filter));
    int count = fileInfo->count();  //文件个数

    // 随机选取文件
    QTime time;
    time= QTime::currentTime();
    qsrand(time.msec() + time.second() * 1000);
    int n = qrand() % count;
    QString fileName = QString(":/puzzles/%1.txt").arg(n + 1);

    QFile file(fileName);
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QVector<int> counts(10, 0);
    for (auto &set : m_numPositions)
    {
        set.clear();
    }

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
            m_numPositions[val].insert(qMakePair(r, c));
            ++counts[val];   
        }
    }

    for (int num = 1; num <= 9; ++num)
    {
        m_counters[num]->setCount(9 - counts[num]);
    }

    m_undoOps.clear();
    m_undoButton->setEnabled(false);
    m_redoButton->setEnabled(false);
}

// 没有结果时的处理
void MainWindow::solve()
{
    if (m_panel->isVisible())
    {
        return;
    }

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

    for (int num = 0; num <= 9; ++num)
    {
        m_numPositions[num].clear();
        m_counters[num]->setCount(0);
    }

    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            int value = solver.m_res[r][c];
            m_grids[r][c]->setValue(value);
            m_numPositions[value].insert(qMakePair(r, c));
        }
    }
}


void MainWindow::redo()
{
    if (m_panel->isVisible())
    {
        return;
    }

    Op op = m_redoOps.pop();
    m_undoOps.push(op);
    changeNumber(op.row, op.col, op.before, op.after);

    m_undoButton->setEnabled(true);
    m_redoButton->setEnabled(m_redoOps.size() > 0);
}

void MainWindow::undo()
{
    if (m_panel->isVisible())
    {
        return;
    }

    Op op = m_undoOps.pop();
    m_redoOps.push(op);
    changeNumber(op.row, op.col, op.after, op.before);

    m_redoButton->setEnabled(true);
    m_undoButton->setEnabled(m_undoOps.size() > 0);
}
