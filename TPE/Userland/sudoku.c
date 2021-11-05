char ****table = [[[[0, 0, 0], [6, 8, 0], [1, 9, 0]], [[2, 6, 0], [0, 7, 0], [0, 0, 4]],
    [[7, 0, 1], [0, 9, 0], [5, 0, 0]]], [[[8, 2, 0], [0, 0, 4],
    [0, 5, 0]], [[1, 0, 0], [6, 0, 2], [0, 0, 3]], [[0, 4, 0], [9, 0, 0], [0, 2, 8]]],
    [[[0, 0, 9], [0, 4, 0], [7, 0, 3]], [[3, 0, 0], [0, 5, 0], [0, 1, 8]],
    [[0, 7, 4], [0, 3, 6], [0, 0, 0]]]];
char remainingCount = 45;

bool tryAddPlay(char number, int rowIndex, int columnIndex) {
    int bigRowIndex = rowIdx / TABLE_SZ;
    int bigColumnIndex = colIdx / TABLE_SZ;
    int smallRowIndex = rowIdx % TABLE_SZ;
    int smallColumnIndex = colIdx % TABLE_SZ;

    if (table[bigRowIndex][bigColumnIndex][smallRowIndex][smallColumnIndex] != 0) return false;

    for (int i = 0; i < TABLE_SZ; i++) {
        for (int j = 0; j < TABLE_SZ; j++) {
            if (table[bigRowIndex][bigColumnIndex][i][j] == number ||
            table[bigRowIndex][i][smallRowIndex][j] == number ||
            table[i][bigColumnIndex][j][smallColumnIndex] == number) {
                return false;
            }
        }
    }

    table[bigRowIndex][bigColumnIndex][smallRowIndex][smallColumnIndex] = number;
    return --remainingCount == 0;
}
