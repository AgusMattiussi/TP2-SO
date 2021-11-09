#include <sudokuHandler.h>

static void initializeColumnInstructionUI();
static void initializeRowInstructionUI();
static void updateColumnInstructionUI();
static void updateRowInstructionUI();

int additionResult;
char columnInstruction;
char rowInstruction;

void startSudoku() {
    clearSudokuScreen();
    additionResult = ADDED_AND_CAN_CONTINUE;
    initializeSudoku();
    drawSudoku(getStartingNumbers());
    columnInstruction = 0;
    rowInstruction = 0;
    initializeColumnInstructionUI();
    initializeRowInstructionUI();
}

void updateSudoku(char digit) {   
    if (additionResult == ADDED_AND_WON) {
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
        additionResult = tryAddPlayForSudoku(number, rowAux, columnAux);
        
        if (additionResult == ADDED_AND_CAN_CONTINUE) {
            drawInSudokuPos(columnAux, rowAux, number + '0');
        } 
        else if(additionResult == ADDED_AND_WON) {
            drawSudokuWinScreen();
            return;
        }
        
        columnInstruction = 0;
        updateColumnInstructionUI();
        rowInstruction = 0;
        updateRowInstructionUI();
    }
}

static void initializeColumnInstructionUI() {
    char *stringForPrint = "Inserting in column: ";

    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 1;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X;

    printInPos(stringForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}

static void initializeRowInstructionUI() {
    char *stringForPrint = "Inserting in row: ";

    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 3;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X;

    printInPos(stringForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}

static void updateColumnInstructionUI() {
    char charForPrint = (columnInstruction != 0) ? columnInstruction + '0' : ' ';
    
    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 1;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X + 21;

    printCharInPos(charForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}

static void updateRowInstructionUI() {
    char charForPrint = (rowInstruction != 0) ? rowInstruction + '0' : ' ';
    
    int rowForPrint = TABLE_UPPER_LEFT_CORNER_POS_Y + TABLE_TOTAL_SIZE * 2 + 3;
    int columnForPrint = TABLE_UPPER_LEFT_CORNER_POS_X + 18;

    printCharInPos(charForPrint, rowForPrint, columnForPrint, INSTRUCTION_TEXT_COLOR_CODE);
}
