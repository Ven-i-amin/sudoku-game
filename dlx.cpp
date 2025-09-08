#include "dlx.hpp"
#include <vector>
#include <stack>
#include <algorithm>

using std::vector;

template <typename T>
Dlx<T>::Dlx(
    vector<T*> rowKeys, vector<T*> colKeys, 
    vector<vector<bool>> matrixValues
) : rowSize(rowKeys.size()), colSize(colKeys.size()), 
nodeMatrix(rowSize, vector<Node*>(colSize, nullptr)),
deleteStep(0)
{
    for (size_t i = 0; i < rowSize; i++) {
        for (size_t j = 0; j < colSize; j++) {
            Node *node = new Node();
            node->rowKey = rowKeys[i];
            node->colKey = colKeys[j];

            nodeMatrix[i][j] = node;
        }
    }

    for (size_t i = 1; i < rowSize; i++) {
        for (size_t j = 1; j < colSize; j++) {
            Node *node = nodeMatrix[i][j];
            node->up = nodeMatrix[i - 1][j];
            node->left = nodeMatrix[i][j - 1];
        }
    }

    for (size_t i = 0; i < rowSize; i++) {
        for (size_t j = 0; j < colSize; j++) {
            Node *node = nodeMatrix[i][j];
            node->down = nodeMatrix[i + 1][j];
            node->right = nodeMatrix[i][j + 1];
        }
    }

    startNode = nodeMatrix[0][0];
}

template <typename T>
Dlx<T>::~Dlx() {
    for (vector<Node*> row : nodeMatrix) {
        for (Node* node : row) {
            delete node;
        }
    }

    delete nodeMatrix;
}

template <typename T>
vector<bool>& Dlx<T>::popRow(int index) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    for (int i = 0; i < index; i++) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {        
        if (index == 0) {
            node->down->up = nullptr;
        } else if (index == colSize) {
            node->up->down = nullptr;
        } else {
            node->down->up = node->up;
            node->up->down = node->down;
        }

        row[i] = node->value;
        node->deleteStep = deleteStep;
        node = node->right;
    }

    rowSize--;

    return row;
}

template <typename T>
vector<bool>& Dlx<T>::popRow(T *rowKey) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    while (node->rowKey != rowKey) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {        
        if (index == 0) {
            node->down->up = nullptr;
        } else if (index == colSize) {
            node->up->down = nullptr;
        } else {
            node->down->up = node->up;
            node->up->down = node->down;
        }

        row[i] = node->value;
        node->deleteStep = deleteStep;
        node = node->right;
    }

    rowSize--;

    return row;
}

template <typename T>
vector<bool>& Dlx<T>::popCol(int index) {
    Node *node = startNode;
    vector<bool> col(colSize);

    for (int i = 0; i < index; i++) {
        node = node->right;
    }

    for (int i = 0; i < rowSize; i++) {        
        if (index == 0) {
            node->right->left = nullptr;
        } else if (index == rowSize) {
            node->left->right = nullptr;
        } else {
            node->right->left = node->left;
            node->left->right = node->right;
        }

        col[i] = node->value;
        node->deleteStep = deleteStep;
        node = node->right;
    }

    colSize--;

    return col;
}

template <typename T>
vector<bool>& Dlx<T>::popCol(T *colKey) {
    Node *node = startNode;
    vector<bool> col(rowSize);

    while (node->colKey != colKey) {
        node = node->right;
    }

    for (int i = 0; i < rowSize; i++) {        
        if (index == 0) {
            node->down->up = nullptr;
        } else if (index == rowSize) {
            node->up->down = nullptr;
        } else {
            node->down->up = node->up;
            node->up->down = node->down;
        }

        col[i] = node->value;
        node->deleteStep = deleteStep;
        node = node->right;
    }

    colSize--;

    return col;
}

template <typename T>
vector<bool>& Dlx<T>::getRow(int index) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    for (int i = 0; i < index) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {
        row[i] = node->value;
        node = node->right;
    }

    return row;
}

template <typename T>
vector<bool>& Dlx<T>::getRow(T *rowKey) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    while (node->rowKey != rowKey) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {
        row[i] = node->value;
        node = node->right;
    }

    return row;
}

template <typename T>
void Dlx<T>::setRow(int index, vector<bool> newValues) {
    Node *node = startNode;

    for (int i = 0; i < index; i++) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->right;
    }

    return row;
}

template <typename T>
void Dlx<T>::setRow(T *rowKey, vector<bool> newValues) {
        Node *node = startNode;

    while(node->rowKey != rowKey) {
        node = node->down;
    }

    for (int i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->right;
    }

    return row;
}

template <typename T>
vector<bool>& Dlx<T>::getCol(int index) {
    Node *node = startNode;
    vector<bool> col(colSize);

    for (int i = 0; i < index; i++) {
        node = node->right;
    }

    for (int i = 0; i < colSize; i++) {
        col[i] = node->value;
        node = node->down;
    }

    return col;
}

template <typename T>
vector<bool> Dlx<T>::getCol(T *colKey) {
    for (size_t j = 0; j < nodeMatrix[0].size(); ++j) {
        if (nodeMatrix[0][j]->colKeys == colKey) return getCol(j);
    }
    return vector<bool>();
}

template <typename T>
void Dlx<T>::setCol(int index, vector<bool> newValues) {
    for (size_t i = 0; i < nodeMatrix.size(); ++i) {
        nodeMatrix[i][index]->value = newValues[i];
    }
}

template <typename T>
void Dlx<T>::setCol(T *colKey, vector<bool> newValues) {
    for (size_t j = 0; j < nodeMatrix[0].size(); ++j) {
        if (nodeMatrix[0][j]->colKeys == colKey) {
            setCol(j, newValues);
            return;
        }
    }
}

template <typename T>
void Dlx<T>::checkpoint() {
    // Здесь можно реализовать сохранение текущего состояния
}

template <typename T>
void Dlx<T>::returnToLastCheckPoint() {
    // Здесь можно реализовать возврат к последнему сохранённому состоянию
}

template <typename T>
Dlx<T>& Dlx<T>::copy() {
    // Создаём глубокую копию
    vector<T*> rowKeys;
    vector<T*> colKeys;
    vector<vector<bool>> matrixValues;

    for (auto &row : nodeMatrix) {
        vector<bool> rowVals;
        for (auto &node : row) {
            rowVals.push_back(node->value);
        }
        matrixValues.push_back(rowVals);
        rowKeys.push_back(row[0]->rowKeys);
    }

    for (auto &node : nodeMatrix[0]) {
        colKeys.push_back(node->colKeys);
    }

    Dlx<T> *newDlx = new Dlx<T>(rowKeys, &colKeys, matrixValues);
    return *newDlx;
}
