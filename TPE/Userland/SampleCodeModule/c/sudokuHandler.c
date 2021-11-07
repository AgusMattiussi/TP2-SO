#include <sudokuHandler.h>

static void updateSudokuUI();

void updateSudoku(char digit) {
    static char instruction[] = {0, 0, 0};

    char number = digit - '0';

    if (instruction[0] == 0) {
        if (number != 0) {
            instruction[0] = number;
        }
    } else if (instruction[1] == 0) {
        if (number != 0) {
            instruction[1] = number;
        } else {
            instruction[0] = 0;
        }
    } else {
        tryAddPlayForSudoku(instruction[0] - 1, instruction[1] - 1, number)
        instruction = {0, 0, 0};
    }

    updateSudokuUI();
}

static void updateSudokuUI() {

}
