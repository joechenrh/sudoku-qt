#include "sudokulogic.h"

SudokuLogic::SudokuLogic() {

}

void SudokuLogic::Clear(Pos pos) {
    this->SelectSingle(pos, 0);
}

std::vector<Pos> SudokuLogic::SelectSingle(Pos pos, int value) {
    std::vector<Pos> conflict;

    int previous = m_board[pos.first][pos.second];
    if (previous > 0) {

    }

    m_board[pos.first][pos.second] = value;
    if (value > 0) {

    }
}

void SudokuLogic::randomGeneratePuzzle() {
    memset(m_board, 0, sizeof(m_board));
    memset(m_editable, 0, sizeof(m_editable));
    m_board[0][0] = 8;
    m_board[1][1] = 7;
    m_board[1][4] = 9;
    m_board[1][6] = 2;
    m_board[2][2] = 3;
    m_board[2][3] = 6;
    m_board[3][1] = 5;
    m_board[3][5] = 7;
    m_board[4][6] = 4;
    m_board[4][5] = 5;
    m_board[4][6] = 7;
    m_board[5][3] = 1;
    m_board[5][7] = 3;
    m_board[6][2] = 1;
    m_board[6][7] = 6;
    m_board[6][8] = 8;
    m_board[7][2] = 8;
    m_board[7][3] = 5;
    m_board[7][7] = 1;
    m_board[8][1] = 9;
    m_board[8][6] = 4;
}
