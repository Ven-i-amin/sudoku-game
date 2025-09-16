#include <gtest/gtest.h>
#include "src/solver/dlx.cpp"

using std::vector;

TEST(DlxTest, MatrixInitilization) {
    vector<int*> rowKeys = {new int(1), new int(2)};
    vector<int*> colKeys = {new int(10), new int(20)};

    vector<std::vector<bool>> matrix = {
        { true, false },
        { false, true }
    };

    Dlx<int> dlx(rowKeys, colKeys, matrix);

    EXPECT_TRUE(dlx.getCell(0, 0));
    EXPECT_FALSE(dlx.getCell(0, 1));
    EXPECT_TRUE(dlx.getCell(1, 0));
    EXPECT_FALSE(dlx.getCell(1, 1));
};

TEST(DlxText, PopAndCheckpointAndCancel) {
        vector<int*> rowKeys = {new int(1), new int(2)};
    vector<int*> colKeys = {new int(10), new int(20)};

    vector<std::vector<bool>> matrix = {
        { true, false },
        { false, true }
    };

    Dlx<int> dlx(rowKeys, colKeys, matrix);
    dlx.popRow(0);
    dlx.checkpoint();

    EXPECT_FALSE(dlx.getCell(0, 0));
    EXPECT_TRUE(dlx.getCell(0, 1));

    dlx.returnToLastCheckpoint();
    
    EXPECT_TRUE(dlx.getCell(0, 0));
    EXPECT_FALSE(dlx.getCell(0, 1));
    EXPECT_TRUE(dlx.getCell(1, 0));
    EXPECT_FALSE(dlx.getCell(1, 1));

    EXPECT_EQ(dlx.getRowSize(), 2);
    EXPECT_EQ(dlx.getColSize(), 2);
}