#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QLabel>
#include <vector>
#include <QFont>
#include <QDir>
#include <QDebug>
#include <QGraphicsDropShadowEffect>

#include "sudokusolver.h"

#define startX 100
#define startY 100
#define gridSize 75
#define spacing 10

struct Op
{
    int row;    // 操作的行数
    int col;    // 操作的列数
    int before; // 更改前的值
};

QPushButton* createButton(QString text)
{
    QFont buttonFont("华文新魏", 15);
    buttonFont.setPointSize(12);
    buttonFont.setBold(true);

    QGraphicsDropShadowEffect *shadow_effect = new QGraphicsDropShadowEffect;
    shadow_effect->setOffset(1, 1);
    shadow_effect->setColor(Qt::gray);
    shadow_effect->setBlurRadius(2);

    QPushButton *button = new QPushButton(text);
    button->setFont(buttonFont);
    button->setGraphicsEffect(shadow_effect);
    button->setStyleSheet("QWidget{border-radius:40px;background-color:#FFFFFF;}"
                          "QPushButton:hover{background-color:rgb(236,236,236);}"
                          "QPushButton:pressed{background-color:rgb(222,222,222);}");
    return button;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // 背景色
    QColor color = QColor("#DDE2E5");
    QPalette p = this->palette();
    p.setColor(QPalette::Window,color);
    this->setPalette(p);

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

            GridWidget *grid = new GridWidget(r, c);
            grid->setParent(this);
            grid->move(startX + c * gridSize + c / 3 * spacing, startY + r * gridSize + r / 3 * spacing);
            m_grids[r][c] = grid;

            connect(grid, &GridWidget::hovered, [=](){changeBackground(r, c); });
            connect(grid, &GridWidget::leave, [=](){recoverBackground(r, c); });
            connect(grid, &GridWidget::clicked, [=](){changeNumber(r, c); });
            connect(grid, &GridWidget::rightClicked, [=](){clearGrid(r, c);});
        }
    }

    QPushButton *loadButton = createButton("load");
    loadButton->setParent(this);
    loadButton->move(startX + (gridSize * 3 + spacing) * 0, gridSize * 9 + 50 + startY);
    loadButton->setFixedSize(gridSize * 3, 80);
    connect(loadButton, &QPushButton::clicked, [&](){loadRandomPuzzle();});

    QPushButton *solveButton = createButton("solve");
    solveButton->setParent(this);
    solveButton->move(startX + (gridSize * 3 + spacing) * 1, gridSize * 9 + 50 + startY);
    solveButton->setFixedSize(gridSize * 3, 80);
    connect(solveButton, &QPushButton::clicked, [&](){solve();});

    QPushButton *clearButton = createButton("clear");
    clearButton->setParent(this);
    clearButton->move(startX + (gridSize * 3 + spacing) * 2, gridSize * 9 + 50 + startY);
    clearButton->setFixedSize(gridSize * 3, 80);
    connect(clearButton, &QPushButton::clicked,  [&](){clearAll();});

    m_panel = new SelectPanel(75, this);

    int space = std::min(spacing / 10, 1);
    for (int i = 0; i < 9; i++)
    {
       Counter *counter = new Counter(i + 1, gridSize, this);
       counter->move(gridSize * 9 + 50 + startX, startY + space + i * (space + gridSize + 1));
       m_counters.push_back(counter);
       connect(counter, &Counter::hovered, [=]() {highlight(i + 1, true);});
       connect(counter, &Counter::leave, [=]() {highlight(i + 1, false);});
    }

    loadRandomPuzzle();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_panel;
}

// 冲突检测 大概是完成了
void MainWindow::clearGrid(int r, int c)
{
    int previous = m_grids[r][c]->getValue();
    qDebug() << previous;
    if (previous > 0)
    {
        for (auto pair : m_controlRanges[r][c])
        {
            if (m_grids[pair.first][pair.second]->getValue() == previous)
            {
                m_grids[pair.first][pair.second]->removeConflict(1);
            }
        }
        m_counters[m_grids[r][c]->getValue() - 1]->plus();
    }
    m_grids[r][c]->setValue(0);
    m_grids[r][c]->clearConflict();
}

// 不要多次调用plus 冲突检测完成
void MainWindow::clearAll()
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (m_grids[r][c]->getEnabled() && m_grids[r][c]->getValue() > 0)
            {
                m_counters[m_grids[r][c]->getValue() - 1]->plus();
                m_grids[r][c]->setValue(0);
            }
            m_grids[r][c]->clearConflict();
        }
    }
}

// 统计每个数的位置，就不用遍历了
void MainWindow::highlight(int num, int active)
{
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            if (m_grids[r][c]->getValue() == num)
            {
                if (active)
                {
                    m_grids[r][c]->hideButton();
                }
                else
                {
                    m_grids[r][c]->revealButton();
                }

            }
        }
    }
}

void MainWindow::loadRandomPuzzle()
{
    QFile file("D:/test.txt");
    if(!file.open(QFile::ReadOnly))
    {
        return;
    }

    QVector<int> counts(10, 0);

    int num;
    QString array = file.readAll();
    QStringList rows = array.split('\n');
    for (int r = 0; r < 9; r++)
    {
        QStringList cols = rows.at(r).split(' ');
        for (int c = 0; c < 9; c++)
        {
            num = cols.at(c).toInt();
            ++counts[num];
            if (num > 0)
            {
                m_grids[r][c]->setEnabled(false);
                m_grids[r][c]->setValue(num);
            }
            else
            {
                m_grids[r][c]->setEnabled(true);
                m_grids[r][c]->setValue(0);
            }
        }
    }

    for (int i = 1; i <= 9; i++)
    {
        m_counters[i - 1]->setValue(9 - counts[i]);
    }
}

// 能不能预计算每一格要显示的范围？ 完成
void MainWindow::recoverBackground(int r, int c)
{
    for (auto pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->revealButton();
    }
    m_grids[r][c]->revealButton();
}

// 同上. 完成
void MainWindow::changeBackground(int r, int c)
{
    for (auto pair : m_controlRanges[r][c])
    {
        m_grids[pair.first][pair.second]->hideButton();
    }
    m_grids[r][c]->hideButton();
}

// 冲突检测 大概是完成了
void MainWindow::changeNumber(int r, int c)
{
    m_panel->move(gridSize * c + startX + c / 3 * spacing, r * gridSize + startY + r / 3 * spacing + 22);
    m_panel->exec();

    int selected = m_panel->number();
    if (selected == 0)
    {
        return;
    }

    int previous = m_grids[r][c]->getValue();
    m_grids[r][c]->setValue(m_panel->number());
    m_counters[selected - 1]->minus();

    // Conflict detection
    if (previous > 0)
    {
        m_counters[previous - 1]->plus();
        for (auto pair : m_controlRanges[r][c])
        {
            if (m_grids[pair.first][pair.second]->getValue() == previous)
            {
                m_grids[pair.first][pair.second]->removeConflict(1);
            }
        }
    }

    int num = 0;
    m_grids[r][c]->clearConflict();
    if (selected > 0)
    {
        for (auto pair : m_controlRanges[r][c])
        {
            if (m_grids[pair.first][pair.second]->getValue() == selected)
            {
                m_grids[pair.first][pair.second]->addConflict(1);
                ++num;
            }
        }
    }
    m_grids[r][c]->addConflict(num);
}

// 没有结果时的处理
void MainWindow::solve()
{
    std::vector<std::vector<int>> puzzle(9, std::vector<int>(9, 0));
    for (int r = 0; r < 9; r++)
    {
        for (int c = 0; c < 9; c++)
        {
            puzzle[r][c] = m_grids[r][c]->getValue();
        }
    }

    SudokuSolver solver(puzzle);
    solver.Solve();

    for (int r = 0; r < 9; r++)
    {
        m_counters[r]->setValue(0);
        for (int c = 0; c < 9; c++)
        {
            m_grids[r][c]->setValue(solver.m_res[r][c]);
        }
    }
}
