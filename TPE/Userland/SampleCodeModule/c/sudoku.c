#include <sudoku.h>

static int isPositionOfStartingNumber(int bigRow, int bigCol, int smallRow, int smallCol);

char startingTable[TABLE_SIZE][TABLE_SIZE][TABLE_SIZE][TABLE_SIZE] =
    {{{{0, 0, 0}, {6, 8, 0}, {1, 9, 0}}, {{2, 6, 0}, {0, 7, 0}, {0, 0, 4}},
    {{7, 0, 1}, {0, 9, 0}, {5, 0, 0}}}, {{{8, 2, 0}, {0, 0, 4},
    {0, 5, 0}}, {{1, 0, 0}, {6, 0, 2}, {0, 0, 3}}, {{0, 4, 0}, {9, 0, 0}, {0, 2, 8}}},
    {{{0, 0, 9}, {0, 4, 0}, {7, 0, 3}}, {{3, 0, 0}, {0, 5, 0}, {0, 1, 8}},
    {{0, 7, 4}, {0, 3, 6}, {0, 0, 0}}}};
char table[TABLE_SIZE][TABLE_SIZE][TABLE_SIZE][TABLE_SIZE];
char remainingCount = 45;

char res[TABLE_TOTAL_SIZE][TABLE_TOTAL_SIZE];

void initializeSudoku() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            for (int k = 0; k < TABLE_SIZE; k++) {
                for (int l = 0; l < TABLE_SIZE; l++) {
                    table[i][j][k][l] = startingTable[i][j][k][l];
                }
            }
        }
    }

    remainingCount = 45;
}

int tryAddPlayForSudoku(char number, int rowIndex, int columnIndex) {
    int bigRowIndex = rowIndex / TABLE_SIZE;
    int bigColIndex = columnIndex / TABLE_SIZE;
    int smallRowIndex = rowIndex % TABLE_SIZE;
    int smallColIndex = columnIndex % TABLE_SIZE;

    if (isPositionOfStartingNumber(bigRowIndex, bigColIndex, smallRowIndex, smallColIndex)) {
        return 0;
    }

    if (number == 0) {
        table[bigRowIndex][bigColIndex][smallRowIndex][smallColIndex] = number;
        return 1;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (table[bigRowIndex][bigColIndex][i][j] == number || table[bigRowIndex][i][smallRowIndex][j] == number || table[i][bigColIndex][j][smallColIndex] == number) {
                return 0;
            }
        }
    }

    table[bigRowIndex][bigColIndex][smallRowIndex][smallColIndex] = number;

    return --remainingCount == 0? 2 : 1;
}

static int isPositionOfStartingNumber(int bigRow, int bigCol, int smallRow, int smallCol) {
    return startingTable[bigRow][bigCol][smallRow][smallCol] ? 1 : 0;
}

char **getStartingNumbers() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            for (int k = 0; k < TABLE_SIZE; k++) {
                for (int l = 0; l < TABLE_SIZE; l++) {
                    res[i * 3 + j][k * 3 + l] = startingTable[i][k][j][l];
                }
            }
        }
    }

    return res;
}

char getNumberInPos(int row, int column) {
    return table[row / 3][column / 3][row % 3][column % 3];
}
