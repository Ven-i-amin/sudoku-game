#pragma once
#include "dlx.tpp"
#include <vector>

using std::vector;

class Solver {
    private:
        static Dlx<vector<int>> fillMatrix(vector<vector<int>> sudoku, size_t squareSize, bool reverseOrder);
    public:
        static vector<vector<int>> solve(vector<vector<int>> sudoku, size_t squareSize, bool reversedOrder);
        static vector<vector<int>> solve(vector<vector<int>> sudoku, size_t squareSize);
};

#include "solver.tpp"