#include <sudokuHandler.h>

// static void updateTableUI();
static void updateColumnInstructionUI();
static void updateRowInstructionUI();

int isPlaying;
char columnInstruction = 0;
char rowInstruction = 0;

void startSudoku() {
    isPlaying = 1;
    initializeSudoku();
    drawSudoku(getStartingNumbers());
    updateColumnInstructionUI();
    updateRowInstructionUI();
}

void updateSudoku(char digit) {
    if (!isPlaying) {
        if (digit == '0') startSudoku();
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
        isPlaying = tryAddPlayForSudoku(number, rowAux, columnAux) ? 0 : 1;
        if (isPlaying) {
            drawInSudokuPos(columnAux, rowAux, getNumberInPos(rowAux, columnAux) + '0');
        } else {
            drawSudokuWinScreen();
        }
        columnInstruction = 0;
        updateColumnInstructionUI();
        rowInstruction = 0;
        updateRowInstructionUI();
    }
}

// static void updateTableUI() {
//     char *rowMarkedNumbers;
//     char *rowHightlightedNumbersPos;
//     char stringForPrint[2];
//     int rowForPrint;
//     int columnForPrint;
//     unsigned char colorCodeForPrint;
//     char isHighlightedNumber;
//     char aux;

//     for (int i = 0; i < TABLE_TOTAL_SIZE; i++) {
//         rowMarkedNumbers = getRowMarkedNumbers(i);
//         rowHightlightedNumbersPos = getRowHighlightedNumbersPos(i);
//         rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + i * 2;
//         aux = 0;

//         for (int j = 0; j < TABLE_TOTAL_SIZE; j++) {
//             if (rowMarkedNumbers[j] != 0) {
//                 stringForPrint[0] = rowMarkedNumbers[j] + '0';
//                 stringForPrint[1] = '\0';
//                 columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X + j * 3;
//                 isHighlightedNumber = (j == rowHightlightedNumbersPos[aux]) ? 1 : 0;
//                 colorCodeForPrint = isHighlightedNumber ? YELLOW_BLACK : WHITE_BLACK;
//                 aux += isHighlightedNumber;
//                 printInPos(stringForPrint, rowForPrint, columnForPrint, colorCodeForPrint);
//             }
//         }
//     }
// }

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