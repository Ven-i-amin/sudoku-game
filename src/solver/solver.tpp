#pragma once
#include "solver.hpp"
#include <map>
#include <set>
#include <climits>

static const int RULE_COUNT = 4;

#pragma region Dlx Creating
Dlx<vector<int>> Solver::fillMatrix(vector<vector<int>> sudoku, size_t squareSize, bool reverseOrder) {
    size_t sudokuSize = sudoku.size();

    vector<vector<bool>> matrix(
        sudokuSize * sudokuSize * sudokuSize, 
        vector<bool>(RULE_COUNT * sudokuSize * sudokuSize, false)
    );
    vector<vector<int>*> rowKeys;
    vector<vector<int>*> colKeys;

    std::map<vector<int>, vector<vector<int>>> rowCol;
    std::set<vector<int>> cols;

    for (int i = 0; i < sudokuSize; i++) {
        for (int j = 0; j < sudokuSize; j++) {
            int square = i / squareSize * squareSize + j / sudokuSize;
            for (int num = 1; num <= sudokuSize; num++) {
                vector<int> row = {i, j, num};
                vector<vector<int>> col;

                if (!sudoku[i][j] || sudoku[i][j] == num) {
                    col = {{0, i, j}, {1, i, num}, {2, j, num}, {3, square, num}};
                } else {
                    col = {{1, i, num}, {2, j, num}, {3, square, num}};
                }

                rowCol[row] = col;

                rowKeys.push_back(&row);
                std::copy(col.begin(), col.end(), std::inserter(cols, cols.end()));
            }
        }
    }

    for (vector<int> reason : cols) {
        colKeys.push_back(&reason);
    }

    for (int i = 0; i < sudokuSize; i++) {
        vector<vector<int>> col;
        std::copy(rowCol[*rowKeys[i]].begin(), rowCol[*rowKeys[i]].end(), std::back_inserter(col));
        for (int j = 0; j < sudokuSize; j++) {
            for (vector<int> oneCol : col) {
                if (oneCol == (*colKeys[j])) {
                    matrix[i][j] = true;
                    col.erase(std::remove(col.begin(), col.end(), oneCol), col.end());
                    break;
                }
            }

            if (!col.size()) {
                break;
            }
        }
    }

    if (reverseOrder) {
        std::reverse(matrix.begin(), matrix.end());
        std::reverse(rowKeys.begin(), rowKeys.end());
    }

    return Dlx<vector<int>>(rowKeys, colKeys, matrix);
}

#pragma endregion

#pragma region Solver

vector<vector<int>> Solver::solve(vector<vector<int>> sudoku, size_t squareSize, bool reverseOrder) {
    size_t sudokuSize = sudoku.size();
    Dlx dancingLinks = fillMatrix(sudoku, squareSize, reverseOrder);
    vector<vector<int>*> answer;
    vector<int> path;

    while(dancingLinks.getRowSize()) {
        path.push_back(0);

        int minTrueCount = INT_MAX;
        vector<bool> minCol;

        for (int x = 0; x < dancingLinks.getRowSize(); x++) {
            int trueCount = 0;
            vector<bool> col = dancingLinks.getCol(x);

            for (bool cell : col) {
                if (cell) {
                    trueCount++;
                }
            }

            if (minTrueCount > trueCount) {
                minTrueCount = trueCount;
                minCol = col;
            }
        }

        if (!minTrueCount) {
            path.pop_back();

            if (!path.size()) {
                break;
            }

            path.back()++;
            answer.pop_back();
            dancingLinks.returnToLastCheckpoint();
            continue;
        }

        int rowIndex = 0;
        int count = 0;

        while (rowIndex < dancingLinks.getColSize()) {
            if (minCol[rowIndex++] && count++ == path.back()) {
                break;
            }
        }

        if (count != path.back()) {
            path.pop_back();

            if (!path.size()) {
                break;
            }

            path.back()++;
            answer.pop_back();
            dancingLinks.returnToLastCheckpoint();
            continue;
        }

        vector<bool> deleteRow = dancingLinks.getRow(rowIndex);
        vector<int> *row;

        for (int x = 0; x < dancingLinks.getRowSize(); x++) {
            if (deleteRow[x]) {
                dancingLinks.popCol(x);
            }
        }

        answer.push_back(dancingLinks.popRow(rowIndex));
        dancingLinks.checkpoint();
    }

    if (answer.size() < sudokuSize * sudokuSize) {
        return vector<vector<int>>(0);
    }

    vector<vector<int>> solvedSudoku(sudokuSize, vector<int>(sudokuSize, 0));

    for (vector<int> *cellInfo : answer) {
        int row = (*cellInfo)[0];
        int col = (*cellInfo)[1];
        int num = (*cellInfo)[2];

        solvedSudoku[row][col] = num;
    }

    return solvedSudoku;
}

vector<vector<int>> Solver::solve(vector<vector<int>> sudoku, size_t squareSize) {
    return solve(sudoku, squareSize, false);
}

#pragma endregion