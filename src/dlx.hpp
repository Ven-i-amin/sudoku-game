#include <vector>

using std::vector;

template <typename T>
class Dlx {
    private:
        enum DeleteReason {
                COL_DELETE,
                ROW_DELETE,
                NONE
        };

        struct Node {
            Node *up;
            Node *right;
            Node *down;
            Node *left;

            T *rowKey;
            T *colKey;

            bool value;

            int deleteStep;
            DeleteReason deleteReason;

            Node() : up(nullptr), right(nullptr), down(nullptr), left(nullptr),
            rowKey(nullptr), colKey(nullptr),
            value(false), deleteStep(-1), deleteReason(NONE) {}
            ~Node() {
                delete deleteReason;
            }
        };

        Node *startNode;
        vector<vector<Node*>> nodeMatrix;
        size_t rowSize;
        size_t colSize;
        int deleteStep;
    public:
        Dlx(vector<T*> rowKeys, vector<T*> colKeys, vector<vector<bool>> matrixValues);

        vector<bool> popRow(int index);
        vector<bool> popRow(T *rowKey);
        vector<bool> popCol(int index);
        vector<bool> popCol(T *colKey);
        
        void checkpoint();
        void returnToLastCheckpoint();

        vector<bool> getRow(int index);
        vector<bool> getRow(T *rowKey);
        void setRow(int index, vector<bool> newValues);
        void setRow(T *rowKey, vector<bool> newValues);

        vector<bool> getCol(int index);
        vector<bool> getCol(T *colKey);
        void setCol(int index, vector<bool> newValues);
        void setCol(T *colKey, vector<bool> newValues);

        bool getCell(int row, int col);
        bool getCell(T *rowKey, T *colKey);
        void setCell(int row, int col, bool value);
        void setCell(T *rowKey, T *colKey, bool value);

        size_t getRowSize();
        size_t getColSize();

        Dlx& copy();
        ~Dlx();
};