#include <sudokuHandler.h>

static void updateColumnInstructionUI();
static void updateRowInstructionUI();

int isPlaying;
char columnInstruction = 0;
char rowInstruction = 0;

void startSudoku() {
    clearSudokuScreen();
    isPlaying = 1;
    initializeSudoku();
    drawSudoku(getStartingNumbers());
    updateColumnInstructionUI();
    updateRowInstructionUI();
}

void updateSudoku(char digit) {   
    if (isPlaying == 2) {
        if (digit == '0') 
            startSudoku();
        return;
    }

    char number = digit - '0';
    char columnAux;
    char rowAux;

    if (columnInstruction == 0) {
        if (number != 0) {
            columnInstruction = number;
            updateColumnInstructionUI();
        }
    } else if (rowInstruction == 0) {
        if (number != 0) {
            rowInstruction = number;
            updateRowInstructionUI();
        } else {
            columnInstruction = 0;
            updateColumnInstructionUI();
        }
    } else {
        columnAux = columnInstruction - 1;
        rowAux = rowInstruction - 1;
        isPlaying = tryAddPlayForSudoku(number, rowAux, columnAux);
        if (isPlaying == 1) {
            drawInSudokuPos(columnAux, rowAux, getNumberInPos(rowAux, columnAux) + '0');
        } 
        else if(isPlaying == 2) {
            drawSudokuWinScreen();
            return;
        }
        columnInstruction = 0;
        updateColumnInstructionUI();
        rowInstruction = 0;
        updateRowInstructionUI();
    }
}

static void updateColumnInstructionUI() {
    char *stringForPrint = "Inserting in column:  ";
    if (columnInstruction != 0) stringForPrint[21] = columnInstruction + '0';

    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 1;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X;

    printInPos(stringForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}

static void updateRowInstructionUI() {
    char *stringForPrint = "Inserting in row:  ";
    if (rowInstruction != 0) stringForPrint[18] = rowInstruction + '0';

    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 3;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X;

    printInPos(stringForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}