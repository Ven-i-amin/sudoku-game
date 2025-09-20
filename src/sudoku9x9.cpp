#include "sudoku.hpp"
#include <random>
#include <vector>

using std::vector;

static const int SUDOKU_SIZE = 9;
static const int SQUARE_SIZE = 3;

static const vector<vector<int>> BOARD_TEMPLATE = {
    {1, 2, 3, 4, 5, 6, 7, 8, 9},
    {7, 8, 9, 1, 2, 3, 4, 5, 6},
    {4, 5, 6, 7, 8, 9, 1, 2, 3},
    {9, 1, 2, 3, 4, 5, 6, 7, 8},
    {6, 7, 8, 9, 1, 2, 3, 4, 5},
    {3, 4, 5, 6, 7, 8, 9, 1, 2},
    {8, 9, 1, 2, 3, 4, 5, 6, 7},
    {5, 6, 7, 8, 9, 1, 2, 3, 4},
    {2, 3, 4, 5, 6, 7, 8, 9, 1}
};

void swap() {
    
}

Sudoku::Sudoku(long long seed) {
    this->seed = seed;
    generateBoard();
    generateChallenge();
}

void Sudoku::generateBoard() {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> distForMove(0, 26);
}