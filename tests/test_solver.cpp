#include <gtest/gtest.h>
#include "src/solver/solver.tpp"

using std::vector;

TEST(solverTest, SolvingSudoku) {
    vector<vector<int>> unsolvedSudoku = {
        {0, 0, 0, 8, 0, 0, 0, 0, 0},
        {4, 0, 0, 0, 1, 5, 0, 3, 0},
        {0, 2, 9, 0, 4, 0, 5, 1, 8},
        {0, 4, 0, 0, 0, 0, 0, 1, 2},
        {0, 0, 6, 0, 0, 0, 0, 0, 0},
        {0, 3, 2, 0, 0, 0, 0, 9, 0},
        {6, 9, 3, 0, 5, 0, 8, 7, 0},
        {0, 5, 0, 4, 8, 0, 3, 0, 1},
        {0, 0, 0, 0, 0, 3, 0, 0, 0}
    };

    vector<vector<int>> solvedSudoku = {
        {3, 1, 5, 8, 2, 7, 9, 4, 6},
        {4, 6, 9, 0, 1, 5, 7, 3, 2},
        {7, 2, 9, 3, 4, 6, 5, 1, 8},
        {9, 4, 5, 6, 3, 8, 1, 2, 7},
        {8, 7, 6, 5, 9, 2, 4, 0, 3},
        {2, 3, 1, 7, 6, 4, 8, 9, 5},
        {6, 9, 3, 2, 5, 1, 0, 7, 4},
        {5, 8, 7, 4, 8, 9, 3, 6, 1},
        {1, 4, 2, 7, 6, 3, 6, 5, 9}
    };

    vector<vector<int>> solvedByClass = Solver::solve(unsolvedSudoku, 3);

    for (int i = 0; i < solvedSudoku.size(); i++) {
        for (int j = 0; j < solvedSudoku[0].size(); j++) {
            EXPECT_EQ(solvedByClass[i][j], solvedSudoku[i][j]);
        }
    }
};