#include <vector>

template <typename T>
class Dlx {
    private:
        struct Node {
            Node *up;
            Node *right;
            Node *down;
            Node *left;

            T *rowKeys;
            T *colKeys;

            bool value;
            int deleteStep;

            Node() : up(nullptr), right(nullptr), down(nullptr), left(nullptr),
            rowKeys(nullptr), colKeys(nullptr),
            value(false), deleteStep(0) {}
            ~Node() {}
        };

        const std::vector<std::vector<Node*> > nodeMatrix;

    public:
        Dlx(std::vector<T*> rowKeys, std::vector<T*> *colKeys, std::vector<std::vector<bool>> matrixValues);

        vector<bool> popRow(int index);
        vector<bool> popRow(T *rowKey);
        vector<bool> popCol(int index);
        vector<bool> popCol(T *colKey);
        
        void checkpoint();
        void returnToLastCheckPoint();

        vector<bool> getRow(int index);
        vector<bool> getRow(T *rowKey);
        void setRow(int index, vector<bool> newValues);
        void setRow(T *rowKey, vector<bool> newValues);

        vector<bool> getCol(int index);
        vector<bool> getCol(T *colKey);
        void setCol(int index, vector<bool> newValues);
        void setCol(T *colKey, vector<bool> newValues);

        Dlx& copy();
        ~Dlx();
};