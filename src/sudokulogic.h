#ifndef SUDOKULOGIC_H
#define SUDOKULOGIC_H

#include <vector>

using Pos = std::pair<int, int>;

class SudokuLogic {
public:
    SudokuLogic();

    // Select single value
    // return conflict value
    std::vector<Pos> SelectSingle(Pos pos, int value);

    void SelectMultiple(Pos pos, int value);

    void Clear(Pos pos);

    void ClearAll();

    bool IsEditble(Pos pos);

    std::vector<Pos> GetPositions(int value);


private:
    void randomGeneratePuzzle();

    int m_board[9][9] = {};

    int m_editable[9][9] = {};
};

#endif // SUDOKULOGIC_H
