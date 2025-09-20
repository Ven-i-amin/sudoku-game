#pragma once
#include "dlx.hpp"
#include <stack>
#include <algorithm>
#include <set>

#pragma region Construct and delete

template <typename T>
Dlx<T>::Dlx(
    vector<T*> rowKeys, vector<T*> colKeys, 
    vector<vector<bool>> matrixValues
) : rowSize(rowKeys.size()), colSize(colKeys.size()), 
nodeMatrix(rowKeys.size(), vector<Node*>(colKeys.size(), nullptr)),
deleteStep(0)
{
    for (size_t i = 0; i < rowSize; i++) {
        for (size_t j = 0; j < colSize; j++) {
            Node *node = new Node();
            node->rowKey = rowKeys[i];
            node->colKey = colKeys[j];
            node->value = matrixValues[i][j];

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

    for (size_t i = 0; i < rowSize - 1; i++) {
        for (size_t j = 0; j < colSize - 1; j++) {
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
}

#pragma endregion

#pragma region Pops

template <typename T>
T* Dlx<T>::popRow(int index) {
    Node *node = startNode;
    DeleteReason reason = ROW_DELETE;
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
        node->deleteReason = reason;
        node = node->right;
    }

    rowSize--;

    if (!index) {
        startNode = startNode->down;
    }

    return node->rowKey;
}

template <typename T>
vector<bool> Dlx<T>::popRow(T *rowKey) {
    Node *node = startNode;
    DeleteReason reason = ROW_DELETE;
    vector<bool> row(rowSize);

    int index = 0;

    while (node->rowKey != rowKey) {
        node = node->down;
        index++;
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
        node->deleteReason = reason;
        node = node->right;
    }

    rowSize--;

    if (startNode->rowKey == rowKey) {
        startNode = startNode->down;
    }

    return row;
}

template <typename T>
T* Dlx<T>::popCol(int index) {
    Node *node = startNode;
    DeleteReason reason = ROW_DELETE;

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

        node->deleteStep = deleteStep;
        node->deleteReason = reason;
        node = node->down;
    }

    colSize--;

    if (!index) {
        startNode = startNode->right;
    }

    return node->colKey;
}

template <typename T>
vector<bool> Dlx<T>::popCol(T *colKey) {
    Node *node = startNode;
    DeleteReason reason = ROW_DELETE;
    vector<bool> col(rowSize);

    int index = 0;

    while (node->colKey != colKey) {
        node = node->right;
        index++;
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
        node->deleteReason = reason;
        node = node->down;
    }

    colSize--;

    if (startNode->colKey == colKey) {
        startNode = startNode->right;
    }

    return col;
}

#pragma endregion

#pragma region Row Getters and Setters
template <typename T>
vector<bool> Dlx<T>::getRow(int index) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    for (size_t i = 0; i < index; i++) {
        node = node->down;
    }

    for (size_t i = 0; i < colSize; i++) {
        row[i] = node->value;
        node = node->right;
    }

    return row;
}

template <typename T>
vector<bool> Dlx<T>::getRow(T *rowKey) {
    Node *node = startNode;
    vector<bool> row(rowSize);

    while (node->rowKey != rowKey) {
        node = node->down;
    }

    for (size_t i = 0; i < colSize; i++) {
        row[i] = node->value;
        node = node->right;
    }

    return row;
}

template <typename T>
void Dlx<T>::setRow(int index, vector<bool> newValues) {
    Node *node = startNode;

    for (size_t i = 0; i < index; i++) {
        node = node->down;
    }

    for (size_t i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->right;
    }
}

template <typename T>
void Dlx<T>::setRow(T *rowKey, vector<bool> newValues) {
        Node *node = startNode;

    while(node->rowKey != rowKey) {
        node = node->down;
    }

    for (size_t i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->right;
    }
}

#pragma endregion

#pragma region Col Getters and Setters
template <typename T>
vector<bool> Dlx<T>::getCol(int index) {
    Node *node = startNode;
    vector<bool> col(colSize);

    for (size_t i = 0; i < index; i++) {
        node = node->right;
    }

    for (size_t i = 0; i < colSize; i++) {
        col[i] = node->value;
        node = node->down;
    }

    return col;
}

template <typename T>
vector<bool> Dlx<T>::getCol(T *colKey) {
    Node *node = startNode;
    vector<bool> col(colSize);

    while (node->colKey == colKey) {
        node = node->right;
    }

    for (size_t i = 0; i < colSize; i++) {
        col[i] = node->value;
        node = node->down;
    }

    return col;
}

template <typename T>
void Dlx<T>::setCol(int index, vector<bool> newValues) {
    Node *node = startNode;
    vector<bool> col(colSize);

    for (size_t i = 0; i < index; i++) {
        node = node->right;
    }

    for (size_t i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->down;
    }
}

template <typename T>
void Dlx<T>::setCol(T *colKey, vector<bool> newValues) {
    Node *node = startNode;
    vector<bool> col(colSize);

    while (node->colKey == colKey) {
        node = node->right;
    }

    for (size_t i = 0; i < colSize; i++) {
        node->value = newValues[i];
        node = node->down;
    }
}

#pragma endregion

#pragma region Cell Getters and Setters

template <typename T>
bool Dlx<T>::getCell(int row, int col) {
    Node *node = startNode;

    for (size_t i = 0; i < row; i++) {
        node = node->down;
    }

    for (size_t i = 0; i < col; i++) {
        node = node->right;
    }

    return node->value;
}

template <typename T>
bool Dlx<T>::getCell(T *rowKey, T *colKey) {
    Node *node = startNode;

    while (node->rowKey != rowKey) {
        node = node->down;
    }

    while (node->colKey != colKey) {
        node = node->right;
    }

    return node->value;
}

template <typename T>
void Dlx<T>::setCell(int row, int col, bool value) {
    Node *node = startNode;

    for (size_t i = 0; i < row; i++) {
        node = node->down;
    }

    for (size_t i = 0; i < col; i++) {
        node = node->right;
    }

    node->value = value;
}

template <typename T>
void Dlx<T>::setCell(T *rowKey, T *colKey, bool value) {
    Node *node = startNode;

    while (node->rowKey != rowKey) {
        node = node->down;
    }

    while (node->colKey != colKey) {
        node = node->right;
    }

    node->value = value;
}

#pragma endregion

#pragma region Get Sizes

template <typename T>
size_t Dlx<T>::getRowSize() {
    return rowSize;
}

template <typename T>
size_t Dlx<T>::getColSize() {
    return colSize;
}

#pragma region Checkpoint

template <typename T>
void Dlx<T>::checkpoint() {
    deleteStep++;
}

template <typename T>
void Dlx<T>::returnToLastCheckpoint() {
    deleteStep--;
    std::set<std::pair<int, DeleteReason>> needToReturn;

    size_t rSize = nodeMatrix.size();
    size_t cSize = nodeMatrix[0].size();

    for (size_t i = 0; i < rSize; i++) {
        for (size_t j = 0; j < cSize; j++) {
            Node *node = nodeMatrix[i][j];

            if (node->deleteStep == deleteStep) {
                if (node->up != nullptr) {
                    node->up->down = node;
                }

                if (node->right != nullptr) {
                    node->right->left = node;
                }

                if (node->down != nullptr) {
                    node->down->up = node;
                }

                if (node->left != nullptr) {
                    node->left->right = node;
                }

                needToReturn.insert(std::pair<int, DeleteReason>(node->deleteStep, node->deleteReason));
                node->deleteStep = -1;
                node->deleteReason = NONE;
            }
        }
    }

    for (std::pair<int, DeleteReason> reason : needToReturn) {
        switch (reason.second) {
            case ROW_DELETE:
            rowSize++;
            break;
            case COL_DELETE:
            colSize++;
            break;
            default:
            throw std::logic_error("Deleting cell without a reason");
        }
    }

    for (size_t i = 0; i < rSize; i++) {
        for (size_t j = 0; j < cSize; j++) {
            if (nodeMatrix[i][j]->deleteReason == NONE) {
                startNode = nodeMatrix[i][j];
                return;
            }
        }
    }
}

#pragma endregion

#pragma region Copy

template <typename T>
Dlx<T>& Dlx<T>::copy() {
    vector<T*> rowKeys;
    vector<T*> colKeys;
    vector<vector<bool>> matrixValues;

    for (vector<Node*> &row : nodeMatrix) {
        vector<bool> rowVals;
        for (Node *node : row) {
            rowVals.push_back(node->value);
        }
        matrixValues.push_back(rowVals);
        rowKeys.push_back(row[0]->rowKeys);
    }

    for (auto &node : nodeMatrix[0]) {
        colKeys.push_back(node->colKeys);
    }

    Dlx<T> *newDlx = new Dlx<T>(rowKeys, colKeys, matrixValues);
    return newDlx;
}

#pragma endregion
