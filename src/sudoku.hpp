#include <vector>

using std::vector;

class Sudoku {
    private:
        long long seed;
        vector<vector<int>> board;
        vector<vector<int>> challenge;

        void generateBoard();
        void generateChallenge();
        bool solvable();
    public:
        Sudoku(long long seed);

        vector<vector<int>> getBoard();
        vector<vector<int>> getChallenge();

        void setSudoku(vector<vector<int>> board, vector<vector<int>> challenge);
};