#include <sudoku.h>

static int isPositionOfStartingNumber(int rowIndex, int columnIndex);

char table[TABLE_SIZE][TABLE_SIZE][TABLE_SIZE][TABLE_SIZE] =
    {{{{0, 0, 0}, {6, 8, 0}, {1, 9, 0}}, {{2, 6, 0}, {0, 7, 0}, {0, 0, 4}},
    {{7, 0, 1}, {0, 9, 0}, {5, 0, 0}}}, {{{8, 2, 0}, {0, 0, 4},
    {0, 5, 0}}, {{1, 0, 0}, {6, 0, 2}, {0, 0, 3}}, {{0, 4, 0}, {9, 0, 0}, {0, 2, 8}}},
    {{{0, 0, 9}, {0, 4, 0}, {7, 0, 3}}, {{3, 0, 0}, {0, 5, 0}, {0, 1, 8}},
    {{0, 7, 4}, {0, 3, 6}, {0, 0, 0}}}};
char startingNumbersByColumnsPerRows[9][4] = {{3, 4, 6, 8}, {0, 1, 4, 7}, {0, 1, 5, 6},
    {0, 1, 3, 7}, {2, 3, 5, 6}, {1, 5, 7, 8}, {2, 3, 7, 8}, {1, 4, 7, 8}, {0, 2, 4, 5}};
char remainingCount = 45;

int tryAddPlayForSudoku(char number, int rowIndex, int columnIndex) {
    if (isPositionOfStartingNumber(rowIndex, columnIndex)) return 0;

    int bigRowIndex = rowIndex / TABLE_SIZE;
    int bigColumnIndex = columnIndex / TABLE_SIZE;
    int smallRowIndex = rowIndex % TABLE_SIZE;
    int smallColumnIndex = columnIndex % TABLE_SIZE;

    if (number == 0) {
        table[bigRowIndex][bigColumnIndex][smallRowIndex][smallColumnIndex] = number;
        return 0;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            if (table[bigRowIndex][bigColumnIndex][i][j] == number ||
            table[bigRowIndex][i][smallRowIndex][j] == number ||
            table[i][bigColumnIndex][j][smallColumnIndex] == number) {
                return 0;
            }
        }
    }

    table[bigRowIndex][bigColumnIndex][smallRowIndex][smallColumnIndex] = number;
    return --remainingCount == 0;
}

static int isPositionOfStartingNumber(int rowIndex, int columnIndex) {
    char aux;

    for (int i = 0; i < 4; i++) {
        aux = startingNumbersByColumnsPerRows[rowIndex][i];
        if (aux > columnIndex) return 0;
        if (aux == columnIndex) return 1;
    }
    
    return 0;
}

char *getRowMarkedNumbers(int row) {
    char res[TABLE_TOTAL_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++) {
        for (int j = 0; j < TABLE_SIZE; j++) {
            res[i * 3 + j] = table[row / 3][i][row % 3][j];
        }
    }

    return res;
}

char *getRowHighlightedNumbersPos(int row) {
    return startingNumbersByColumnsPerRows[row];
}