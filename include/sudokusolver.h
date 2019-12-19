/**
 * @file sudokusolver.h
 * @brief Main class for generate and solve sudoku
 * @author Joe chen <joechenrh@gmail.com>
 */


#include <QVector>
#include <QDebug>

#include <iostream>

enum resType {SOLVED, UNSOLVED, FAILED};

int getV9(int v)
{
    switch (v) {
    case 1:
        return 1;
    case 2:
        return 2;
    case 4:
        return 3;
    case 8:
        return 4;
    case 16:
        return 5;
    case 32:
        return 6;
    case 64:
        return 7;
    case 128:
        return 8;
    case 256:
        return 9;
    default:
        break;
    }
    return 0;
}

// 记录格子
struct Grid
{
    int value;
    int count;
    Grid(int _value = 511) : value(_value), count(bitCount(value)) { }

    // 删除若干位，返回实际删除的位数
    int remove(int v)
    {
        int removeCount = bitCount(v & value);
        count -= removeCount;
        // value &= (~v);
        value &= (0x1ff ^ v);
        return removeCount;
    }

    // 统计1的个数
    int bitCount(int v)
    {
        int count{ 0 };
        while (v)
        {
            v &= (v - 1);
            ++count;
        }
        return count;
    }

    // 设置结果
    void setResult(int v)
    {
        value = v;
        count = 1;
    }

    // 返回最高位
    int highest()
    {
        int tmp = value;
        tmp |= (tmp >> 1);
        tmp |= (tmp >> 2);
        tmp |= (tmp >> 4);
        tmp |= (tmp >> 8);
        return tmp - (tmp >> 1);
    }
};

class SudokuSolver
{

    typedef QVector<QVector<Grid>> puzzleType;

public:

    SudokuSolver(QVector<QVector<int>> puzzle)
    {
        m_res = QVector<QVector<int>>(9, QVector<int>(9, 0));
        m_puzzle = puzzleType(9, QVector<Grid>(9));
        for (int r = 0; r < 9; r++)
        {
            for (int c = 0; c < 9; c++)
            {
                if (puzzle[r][c] > 0)
                {
                    m_puzzle[r][c].setResult(1 << (puzzle[r][c] - 1));
                }
            }
        }
    }

    void Solve()
    {
        m_num = 0;
        Solve(m_puzzle);
    }

    QVector<QVector<int>> m_res;

    int m_num;

private:

    void Solve(puzzleType &puzzle)
    {
        if (m_num > 0)
        {
            return;
        }

        reduce(puzzle);
        resType res = checkResult(puzzle);

        if (res == SOLVED) //
        {
            for (int i = 0; i < 9; i++)
            {
                for (int j = 0; j < 9; j++)
                {
                    m_res[i][j] = getV9(puzzle[i][j].value);
                }
            }
            ++m_num;
            printf("Solved");
        }
        else if (res == UNSOLVED)
        {

            int r = 0, c = 0;
            smallestGrid(puzzle, r, c);
            int count = puzzle[r][c].count;
            for (int i = 0; i < count; i++)
            {
                printf("Change %d/%d times: %d\n", r, c, count);

                int highest = puzzle[r][c].highest();
                puzzle[r][c].remove(highest);

                puzzleType copy = puzzleType(9, QVector<Grid>(9));
                copyData(puzzle, copy);
                copy[r][c].setResult(highest);
                Solve(copy);
            }
        }
    }

    void smallestGrid(const puzzleType &puzzle, int &r, int&c)
    {
        int count = 10;
        for (int i = 0; i < 9; i++)
        {
            for (int j = 0; j < 9; j++)
            {
                if (puzzle[i][j].count == 2)
                {
                    r = i;
                    c = j;
                    return;
                }
                else if (puzzle[i][j].count > 2 && puzzle[i][j].count < count)
                {
                    count = puzzle[i][j].count;
                    r = i;
                    c = j;
                }
            }
        }
    }

    // 统计每一个格子的可行解，直至不能再进行为止
    void reduce(puzzleType &puzzle)
    {
        bool changed;
        do
        {
            changed = false;
            for (int r = 0; r < 9; r++)
            {
                for (int c = 0; c < 9; c++)
                {
                    if (puzzle[r][c].count == 1)
                    {
                        continue;
                    }
                    if (reduceGrid(puzzle, r, c))
                    {
                        changed = true;
                    }
                }
            }
        } while (changed);
    }

    // 统计当前格子的可行解，如果可行解改变，返回true，否则返回false
    bool reduceGrid(puzzleType &puzzle, int r, int c)
    {
        int rows{ 0 };
        int cols{ 0 };
        int blocks{ 0 };

        for (int i = 0; i < 9; i++)
        {
            if (puzzle[i][c].count == 1)
            {
                cols += puzzle[i][c].value;
            }
            if (puzzle[r][i].count == 1)
            {
                rows += puzzle[r][i].value;
            }
        }

        for (int i = r / 3 * 3; i < r / 3 * 3 + 3; i++)
        {
            for (int j = c / 3 * 3; j < c / 3 * 3 + 3; j++)
            {
                if (puzzle[i][j].count == 1)
                {
                    blocks += puzzle[i][j].value;
                }
            }
        }

        int invalid = rows | cols | blocks; // 不可选的值
        return puzzle[r][c].remove(invalid) > 0; // 删除的元素大于0表示可行解发生了改变
    }

    // 检查是否完成或失败
    resType checkResult(puzzleType &puzzle)
    {
        for (int i = 0; i < 9; i++)
        {
            int rows{ 0 };
            int cols{ 0 };
            int blocks{ 0 };

            for (int j = 0; j < 9; j++)
            {
                if (puzzle[i][j].count > 1)
                {
                    return UNSOLVED;
                }
                rows |= puzzle[i][j].value;
                cols |= puzzle[j][i].value;
            }
            for (int bc = i / 3 * 3; bc < i / 3 * 3 + 3; bc++)
            {
                for (int br = i % 3 * 3; br < i % 3 * 3 + 3; br++)
                {
                    blocks |= puzzle[bc][br].value;
                }
            }

            if (rows != 0x1ff || cols != 0x1ff || blocks != 0x1ff) {
                return FAILED;
            }
        }
        return SOLVED;
    }

    void copyData(const puzzleType &src, puzzleType &tgt)
    {
        for (int i = 0; i < 9; i++)
        {
            std::copy(src[i].begin(), src[i].end(), tgt[i].begin());
        }
    }

    puzzleType m_puzzle; // 谜题解答
};


