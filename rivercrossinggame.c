/* This program allows the user to play an instance of the River Crossing game.
    Besides this, this program allows the user to find the Expected Duration of an
    initial position, or find the probability of the game outcome of two 
    initial positions.

    Author: Nikko Gammad
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

//function declarations
void addChip(int[], int[], int);
int buildRollCombo(int[], int[], int[], int[], int[]);
double combination(int, int);
void copyPosition(int[], int[], int[], int[]);
int countChips(int[]);
void displayBoard(int[], int[]);
void displayTitle();
void docks(int[], int[]);
void edCompute(int[], int[], int[], int[], int[], int[], int[], int[], int[], double[]);
double expectedDurationRecursive(int[], int[], double[]);
double expectedDurationSimulation(int[], int[]);
double factorial(int);
void findMiddle(int[]);
void gameInputChips(int[], int[]);
void gCompute(int[], int[], int[], int[], int[], int[], int[], int[], int[], double[], double[]);
void gFunctionNonRecursive(int[], int[], int[], int[], double[], double[]);
void gFunctionRecursive(int[], int[], int[], int[], double[], double[]);
void gFunctionSimulation(int[], int[], int[], int[], double[]);
void initializePositions(int[], int[], int[], int[], int[], int[], int[], int[], int[]);
void inputSettings();
bool isSamePosition(int[], int[]);
bool isSameRollCombo(int[], int[]);
int maxDock(int[]);
void nextPosition(int[], int[], int[]);
void nextRollCombo(int[], int);
void playGame(int[], int[], int[], int[], double[]);
void posToString(int[], char[], int);
int powInt(int, int);
void printSettings();
void removeChip(int[], int[], int);
int rollDice();
void rollComboIteration(int[], int[], int, int[], int[], int[], int[], double[], double[]);
void setupDockOrder(int[]);
void setupProbabilities(double[], int);

//constants
const int MAX_CHIPS_DOCKS = 35;
const char DOCK_STRING[35] = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

//enums
enum Player {
    PLAYER_1,
    PLAYER_2
};
enum Mode {
    MODE_GAME, //play the game
    MODE_GRECURSIVE, //find optimal initial position using recursive g Function
    MODE_GNONRECURSIVE, //using non-recursive g Function
    MODE_GSIMULATION, //using simulation
    MODE_GALL, //all three g function methods
    MODE_EDRECURSIVE, //find best expected duration using recursive ed Function
    MODE_EDSIMULATION, //using simulation
    MODE_EDBOTH, //both ed function methods
    MODE_GROUNDROBIN //all positions against all other positions
};
enum Amount {
    AMOUNT_ONE = 1,
    AMOUNT_ALL = 2
};
enum Speed {
    SPEED_1 = 1, //no skips
    SPEED_2 = 2, //only with chips in the middle
    SPEED_3 = 3, //no gaps
    SPEED_4 = 4, //symmetric
    SPEED_5 = 5, //most of chips in the middle
    SPEED_6 = 6 //reduced positions
};
enum Interval { //used for number of simulations and for intervals between console updates when computing for all positions.
    INTERVAL_EXTRATINY = 1,
    INTERVAL_TINY = 10,
    INTERVAL_SMALL = 100,
    INTERVAL_MEDIUM = 1000,
    INTERVAL_BIG = 10000,
    INTERVAL_HUGE = 100000,
    INTERVAL_MASSIVE = 1000000   
};
enum ToStringMode { //used for converting position to string.
    STRING_CHIPS,
    STRING_DOCKS
};

//global variables
int dice = 2;
int faces = 6;
int current_chips = 4;
int current_docks = 12;
int usable_docks = 11;
int middle[2];
int positionCounter = 0;
double maxPositions = 0;
double middleChipPositions = 0;
int noGapPositions = 0;
int symmetricPositions = 0;
int maxMiddlePositions = 0;
int reducedPositions = 0;
enum Player currentPlayer = PLAYER_1;
enum Mode currentMode = MODE_GAME;
enum Amount currentAmount = AMOUNT_ONE;
enum Speed currentSpeed = SPEED_1;
enum Interval currentInterval = INTERVAL_MEDIUM;

int main() {
    //local variables
    char choice1 = '3'; //choice between playing the game or doing the calculations.

    srand(time(0));

    displayTitle();

    //initial input
    //1: Play the game
    //2: Computation and Analysis
    printf("Type [1] to play the original game.\n");
    printf("Type [2] for the computations and analyses.\n");
    printf("Your choice: ");
    scanf(" %c", &choice1);

    //check for invalid input
    while (choice1 != '1' && choice1 != '2' && choice1 != 'g' && choice1 != 'x') {
        printf("Invalid input. Your choice: ");
        scanf(" %c", &choice1);
    }

    //play the original game
    if (choice1 == '1') {
        int pos1[current_chips];
        int pos1Docks[current_docks];
        int pos2[current_chips];
        int pos2Docks[current_docks];
        int pos1Copy[current_chips];
        int pos1CopyDocks[current_docks];
        int pos2Copy[current_chips];
        int pos2CopyDocks[current_docks];
        double probabilities[current_docks];
        int dockOrder[usable_docks];

        initializePositions(pos1, pos1Docks, pos2, pos2Docks, pos1Copy, pos1CopyDocks, pos2Copy, pos2CopyDocks, dockOrder);
        setupProbabilities(probabilities, dice);
        //get both positions
        for (int i = 0; i < current_chips * 2; i++) {
            displayTitle();
            displayBoard(pos1Docks, pos2Docks);
            printf("Chip %d\n", (i / 2) + 1);
            if (currentPlayer == PLAYER_1) {
                gameInputChips(pos1, pos1Docks);
            }
            else {
                gameInputChips(pos2, pos2Docks);
            }

            //switch player
            if (currentPlayer == PLAYER_1) {
                currentPlayer = PLAYER_2;
            }
            else {
                currentPlayer = PLAYER_1;
            }
        }
        //final board, ready to play the game
        playGame(pos1, pos1Docks, pos2, pos2Docks, probabilities);
    }
    //computation and analyses
    else if (choice1 == '2') {
        inputSettings();
        printSettings();

        //initialize these after input.
        int leaderPos[current_chips]; //this holds the data for the leader position.
        int leaderDocks[current_docks];
        int mirrorPos[current_chips]; //holds the data for a position which mirrors the leader.
        int mirrorDocks[current_docks];
        int position[current_chips]; //this holds the data for the current position being checked.
        int positionDocks[current_docks];
        int finalPos[current_chips]; //this holds the data for the final position.
        int finalDocks[current_docks];
        int dockOrder[usable_docks]; //holds the data for the order of docks for moving through positions.
        double probabilities[current_docks]; //holds the probability values for different dice rolls.
        double gFunction[3];

        findMiddle(middle);
        setupDockOrder(dockOrder);
        initializePositions(leaderPos, leaderDocks, mirrorPos, mirrorDocks, position, positionDocks, finalPos, finalDocks, dockOrder);
        setupProbabilities(probabilities, dice);

        if (currentMode >= 1 && currentMode <= 4) { //g function
            gCompute(leaderPos, leaderDocks, mirrorPos, mirrorDocks, position, positionDocks, finalPos, finalDocks, dockOrder, probabilities, gFunction);
        }
        else if (currentMode >= 5 && currentMode <= 7) { //ed function
            edCompute(leaderPos, leaderDocks, mirrorPos, mirrorDocks, position, positionDocks, finalPos, finalDocks, dockOrder, probabilities);
        }
    }
    //function testing
    else if (choice1 == 'g') {
        printf("Welcome to function testing mode\n");
        printf("Here is where I place all my function tests.\n");
        //remember to remove this in the final version
        //unless i use this for roundrobin mode

        current_chips = 4;
        current_docks = 12;
        int pos1[4] = {5, 6, 7, 8};
        int pos1Docks[12] = {0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};
        int pos2[4] = {5, 6, 7, 7};
        int pos2Docks[12] = {0, 0, 0, 0, 1, 1, 2, 0, 0, 0, 0, 0};
        double probabilities[current_docks];
        setupProbabilities(probabilities, dice);

        double gFunction[3];
        gFunctionRecursive(pos1, pos1Docks, pos2, pos2Docks, probabilities, gFunction);
        printf("Recursive: g(A, B) = {%lf, %lf, %lf}\n", gFunction[0], gFunction[1], gFunction[2]);
        gFunctionSimulation(pos1, pos1Docks, pos2, pos2Docks, gFunction);
        printf("Simulation: g(A, B) = {%lf, %lf, %lf}\n", gFunction[0], gFunction[1], gFunction[2]);
        gFunctionNonRecursive(pos1, pos1Docks, pos2, pos2Docks, probabilities, gFunction);
        printf("Non-Recursive: g(A, B) = {%lf, %lf, %lf}\n", gFunction[0], gFunction[1], gFunction[2]);

        currentMode = MODE_GROUNDROBIN;
    }
    else if (choice1 == 'x') {
        printf("Exit the program\n");
    }
    return 0;
}

//adds a chip to the dock indicated by int dock.
//assumes that dock is valid.
//dock counting starts at 0.
void addChip(int pos[], int posDocks[], int dock) {
    //add to pos. replace the first zero
    for (int i = 0; i < current_chips; i++) {
        if (pos[i] == 0) {
            pos[i] = dock + 1;
            break;
        }
    }

    //add to posDocks. increment the dock
    posDocks[dock]++;
}

//builds the roll combo that will clear the board based on pos1Docks and pos2Docks.
int buildRollCombo(int pos1Docks[], int pos2Docks[], int output1[], int output2[], int output3[]) {
    int idx = 0;
    
    //clear output
    for (int i = 0; i < current_chips * 2; i++) {
        output1[i] = 0;
        output2[i] = 0;
    }

    for (int i = 0; i < current_docks; i++) {
        output3[i] = 0;
    }

    //fill up output1 with chips to be cleared.
    //fill up output3 with chips in docks.
    for (int i = 0; i < current_docks; i++) {
        if (pos1Docks[i] > 0 || pos2Docks[i] > 0) {
            output3[i] = pos1Docks[i];
            if (pos1Docks[i] >= pos2Docks[i]) { //player 1 has more or equal chips
                for (int j = 0; j < pos1Docks[i]; j++) {
                    output1[idx] = i + 1;
                    idx++;
                }
            }
            else { //player 2 has more chips
            output3[i] = pos2Docks[i];
                for (int j = 0; j < pos2Docks[i]; j++) {
                    output1[idx] = i + 1;
                    idx++;
                }
            }
        }
    }

    //fill up output2 with the reverse of output1
    for (int i = 0; i < idx; i++) {
        output2[i] = output1[idx - i - 1];
    }

    return idx;
}

//computes for the number of combinations of num1 things taken num2 at a time.
double combination(int num1, int num2) {
    double combi = 1;

    //numerator
    combi *= factorial(num1);
    //denominator
    combi /= (factorial(num1 - num2) * factorial(num2));

    return combi;
}

//copies pos and posDocks to posCopy and posCopyDocks.
void copyPosition(int pos[], int posDocks[], int posCopy[], int posCopyDocks[]) {
    //copy pos to posCopy
    for (int i = 0; i < current_chips; i++) {
        posCopy[i] = pos[i];
    }

    //copy posDocks to posCopyDocks
    for (int i = 0; i < current_docks; i++) {
        posCopyDocks[i] = posDocks[i];
    }
}

//counts the chips in a certain position.
int countChips(int docks[]) {
    int count = 0;

    for (int i = 0; i < current_docks; i++) {
        count += docks[i];
    }

    return count;
}

//displays the board for the River Crossing Game.
void displayBoard(int docks1[], int docks2[]) {
    int maxDockChips1 = maxDock(docks1);
    int maxDockChips2 = maxDock(docks2);

    //upper docks
    for (int i = 0; i < current_chips; i++) {
        if (current_chips - i <= maxDockChips1) {
            printf("|");
        }
        for (int j = 0; j < current_docks; j++) {
            if (current_chips - i <= docks1[j]) {
                printf(" (O) |");
            }
            else if (current_chips - i <= maxDockChips1) {
                printf("     |");
            }
        }
        if (current_chips - i <= maxDockChips1) {
            printf("\n");
        }
    }

    // dock numbers and river
    printf("~");
    for (int i = 0; i < current_docks; i++) {
        printf("~~~~~~");
    }
    printf("\n");
    printf("|");
    for (int i = 0; i < current_docks; i++) {
        printf("  %c  |", DOCK_STRING[i]);
    }
    printf("\n");
    printf("~");
    for (int i = 0; i < current_docks; i++) {
        printf("~~~~~~");
    }
    printf("\n");

    //lower docks
    for (int i = 0; i < current_chips; i++) {
        if (i + 1 <= maxDockChips2) {
            printf("|");
        }
        for (int j = 0; j < current_docks; j++) {
            if (i + 1 <= docks2[j]) {
                printf(" (O) |");
            }
            else if (i + 1 <= maxDockChips2) {
                printf("     |");
            }
        }
        if (i + 1 <= maxDockChips2) {
            printf("\n");
        }
    }
}

//displays the title.
void displayTitle() {
    printf("-------------------\n");
    printf("River Crossing Game\n");
    printf("-------------------\n\n");
}

//converts a position in chips format to its docks format.
void docks(int pos[], int posDocks[]) {
    //clear posDocks
    for (int i = 0; i < current_docks; i++) {
        posDocks[i] = 0;
    }

    //fill up posDocks with chips
    for (int i = 0; i < current_chips; i++) {
        posDocks[pos[i] - 1]++;
    }
}

//computes the expected duration of one or all positions.
void edCompute(int leaderPos[], int leaderDocks[], int mirrorPos[], int mirrorDocks[], int pos[], int posDocks[], int finalPos[], int finalDocks[], int dockOrder[], double prob[]) {
    double ed = 0;
    if (currentAmount == AMOUNT_ONE) {
        int ctr = 0;
        //ask for user input for positions
        //get both positions
        for (int i = 0; i < current_chips; i++) {
            displayTitle();
            displayBoard(leaderDocks, posDocks);

            ctr = i + 1;
            printf("Chip %d\n", ctr);
            gameInputChips(leaderPos, leaderDocks);
        }
        //display final board
        displayBoard(leaderDocks, posDocks);

        //compute
        printf("-----\n");
        switch (currentMode) {
            case (MODE_EDRECURSIVE):
                ed = expectedDurationRecursive(leaderPos, leaderDocks, prob);
                printf("Recursive Expected Duration Function: ed(A) = %lf\n", ed);
                break;
            case (MODE_EDSIMULATION):
                ed = expectedDurationSimulation(leaderPos, leaderDocks);
                printf("Simulation Expected Duration Function: ed(A) = %lf\n", ed);
                break;
            case (MODE_EDBOTH):
                ed = expectedDurationRecursive(leaderPos, leaderDocks, prob);
                printf("Recursive Expected Duration Function: ed(A) = %lf\n", ed);
                ed = expectedDurationSimulation(leaderPos, leaderDocks);
                printf("Simulation Expected Duration Function: ed(A) = %lf\n", ed);
                break;
            default:
                printf("Invalid mode.\n");
        }
    }
    else { //currentAmount == AMOUNT_ALL

    }
}

//computes the expected duration of position pos using the recursive method.
double expectedDurationRecursive(int pos[], int posDocks[], double prob[]) {
    double ed = 1;
    int count = countChips(posDocks);
    int posCopy[current_chips];
    int posCopyDocks[current_docks];
    double sum_prob = 0;

    //trivial cases
    //chip on docks that are impossible to roll
    //assign -1 to ed for the infinite ED
    if (dice > 1) {
        for (int i = 0; i < dice - 1; i++) {
            if (posDocks[i] > 0) {
                return -1;
            }
        }
    }
    //trivial cases
    //no chips
    if (count == 0) {
        return 0;
    }
    //trivial cases
    //one chip
    if (count == 1) {
        ed = 1 / prob[pos[0] - 1];
    }
    //non-trivial case
    else {
        //get the sum of probabilities
        for (int i = 0; i < current_docks; i++) {
            if (posDocks[i] > 0) {
                sum_prob += prob[i];
            }
        }

        //use recursion to get the ed of reduced position
        for (int i = 0; i < current_docks; i++) {
            copyPosition(pos, posDocks, posCopy, posCopyDocks);

            if (posDocks[i] > 0) {
                removeChip(posCopy, posCopyDocks, i);
                ed += prob[i] * expectedDurationRecursive(posCopy, posCopyDocks, prob);
            }
        }

        //divide by sum_prob
        ed /= sum_prob;
    }

    return ed;
}

//computes expected duration using simulation
double expectedDurationSimulation(int pos[], int posDocks[]) {
    double ed = 0;
    int rolls = 0;
    int diceRoll = 0;
    int count = 0;
    int posCopy[current_chips];
    int posCopyDocks[current_docks];

    //trivial case: chip in impossible dock
    if (dice > 1) {
        for (int i = 0; i < dice - 1; i++) {
            if (posDocks[i] > 0) {
                return -1;
            }
        }
    }

    for (int i = 0; i < currentInterval; i++) {
        copyPosition(pos, posDocks, posCopy, posCopyDocks);
        count = countChips(posCopyDocks);

        while (count > 0) {
            diceRoll = rollDice();

            removeChip(posCopy, posCopyDocks, diceRoll - 1);
            rolls++;

            count = countChips(posCopyDocks);
        }
    }

    ed = (double)rolls / (double)currentInterval;

    return ed;
}

//computes for the factorial of the integer num
double factorial(int num) {
    double fact = 1;

    //trivial case
    if (num == 1 || num == 0) {
        fact = 1;
    }
    //non-trivial case. use recursion
    else if (num > 0) {
        fact = (double)num * factorial(num - 1);
    }
    //invalid value of num
    else {
        printf("Invalid number\n");
    }

    return fact;
}

//finds the middle dock/s based on current_docks and dice.
//dock counting starts at 0.
void findMiddle(int middle[]) {
    if (usable_docks % 2 == 1) { //odd
        middle[0] = ((current_docks + dice) / 2) - 1;
        middle[1] = middle[0]; //they are the same
    }
    else { //even
        middle[0] = ((current_docks + dice - 1) / 2) - 1;
        middle[1] = ((current_docks + dice + 1) / 2) - 1;
    }
}

//lets the user add chips to the board.
//dock counting starts at 0.
void gameInputChips(int pos[], int posDocks[]) {
    char inputDock = 'x';
    int dock = -1;

    //ask for input
    printf("Player %d:\n", currentPlayer + 1);
    printf("On which dock would you like your chip to be placed? ");
    scanf(" %c", &inputDock);

    //find in DOCK_STRING
    for (int i = 0; i < current_docks; i++) {
        if (DOCK_STRING[i] == inputDock) {
            dock = i;
        }
    }

    //check for invalid input
    while (dock == -1) {
        printf("Invalid dock number.\n");
        printf("On which dock would you like your chip to be placed? ");
        scanf(" %c", &inputDock);

        //find in DOCK_STRING
        for (int i = 0; i < current_docks; i++) {
            if (DOCK_STRING[i] == inputDock) {
                dock = i;
            }
        }
    }

    //add chips
    addChip(pos, posDocks, dock);
}

//performs the g function analysis based on the settings provided.
void gCompute(int leaderPos[], int leaderDocks[], int mirrorPos[], int mirrorDocks[], int pos[], int posDocks[], int finalPos[], int finalDocks[], int dockOrder[], double prob[], double output[]) {
    char posString[current_chips];
    char docksString[current_docks];
    
    if (currentAmount == AMOUNT_ONE) {
        int ctr = 0;
        //ask for user input for positions
        //get both positions
        for (int i = 0; i < current_chips * 2; i++) {
            displayTitle();
            displayBoard(leaderDocks, posDocks);

            //switch player
            if (i < current_chips) {
                currentPlayer = PLAYER_1;
                ctr = i + 1;
            }
            else {
                currentPlayer = PLAYER_2;
                ctr = i + 1 - current_chips;
            }
            printf("Chip %d\n", ctr);
            if (currentPlayer == PLAYER_1) {
                gameInputChips(leaderPos, leaderDocks);
            }
            else {
                gameInputChips(pos, posDocks);
            }
        }
        //display final board
        displayBoard(leaderDocks, posDocks);

        //compute
        printf("-----\n");
        switch (currentMode) {
            case (MODE_GRECURSIVE):
                gFunctionRecursive(leaderPos, leaderDocks, pos, posDocks, prob, output);
                printf("Recursive g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                break;
            case (MODE_GNONRECURSIVE):
                gFunctionNonRecursive(leaderPos, leaderDocks, pos, posDocks, prob, output);
                printf("Non-Recursive g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                break;
            case (MODE_GSIMULATION):
                gFunctionSimulation(leaderPos, leaderDocks, pos, posDocks, output);
                printf("Simulation g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                break;
            case (MODE_GALL):
                gFunctionRecursive(leaderPos, leaderDocks, pos, posDocks, prob, output);
                printf("Recursive g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                gFunctionNonRecursive(leaderPos, leaderDocks, pos, posDocks, prob, output);
                printf("Non-Recursive g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                gFunctionSimulation(leaderPos, leaderDocks, pos, posDocks, output);
                printf("Simulation g Function: g(A, B) = {%lf, %lf, %lf}\n", output[0], output[1], output[2]);
                break;
            default:
                printf("Invalid mode.\n");
        }
    }
    else { //currentAmount == AMOUNT_ALL
        positionCounter = 0;
        while (!isSamePosition(posDocks, finalDocks)) {
            posToString(pos, posString, STRING_CHIPS);
            printf("%7d | %s\n", positionCounter + 1, posString);
            nextPosition(pos, posDocks, dockOrder);
        }
        //final position
        posToString(pos, posString, STRING_CHIPS);
        printf("%7d | %s\n", positionCounter + 1, posString);
    }
}

//computes the probabilities of the game outcomes using a non-recursive method
void gFunctionNonRecursive(int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double prob[], double output[]) {
    int count1 = countChips(pos1Docks);
    int count2 = countChips(pos2Docks);
    int pos1Copy[current_chips];
    int pos1CopyDocks[current_docks];
    int pos2Copy[current_chips];
    int pos2CopyDocks[current_docks];
    
    //initialize output
    output[0] = 0;
    output[1] = 0;
    output[2] = 0;

    //trivial cases
    //player 1 wins
    if (count1 == 0 && count2 > 0) {
        output[0] = 1;
        output[1] = 0;
        output[2] = 0;
    }
    //player 2 wins
    else if (count1 > 0 && count2 == 0) {
        output[0] = 0;
        output[1] = 1;
        output[2] = 0;
    }
    //tie game
    else if (count1 == 0 && count2 == 0) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    }
    //same positions
    else if (isSamePosition(pos1Docks, pos2Docks)) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    }
    //non-trivial case: more than one chip on one of the positions
    else {
        int rollCombo[current_chips * 2];
        int finalCombo[current_chips * 2];
        int combinedDocks[current_docks];
        int combinedDocksCopy[current_docks];
        int rollCount = buildRollCombo(pos1Docks, pos2Docks, rollCombo, finalCombo, combinedDocks);

        while (!isSameRollCombo(rollCombo, finalCombo)) {
            copyPosition(pos1, pos1Docks, pos1Copy, pos1CopyDocks);
            copyPosition(pos2, pos2Docks, pos2Copy, pos2CopyDocks);

            for (int i = 0; i < current_docks; i++) {
                combinedDocksCopy[i] = combinedDocks[i];
            }

            rollComboIteration(rollCombo, combinedDocksCopy, rollCount, pos1, pos1CopyDocks, pos2Copy, pos2CopyDocks, prob, output);
            nextRollCombo(rollCombo, rollCount);
        }

        //final rollCombo
        copyPosition(pos1, pos1Docks, pos1Copy, pos1CopyDocks);
        copyPosition(pos2, pos2Docks, pos2Copy, pos2CopyDocks);

        for (int i = 0; i < current_docks; i++) {
            combinedDocksCopy[i] = combinedDocks[i];
        }

        rollComboIteration(rollCombo, combinedDocksCopy, rollCount, pos1, pos1CopyDocks, pos2Copy, pos2CopyDocks, prob, output);
    }
}

//computes the probabilities of the game outcomes (g Function) for positions pos1 and pos2.
void gFunctionRecursive(int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double prob[], double output[]) {
    double gValues_sub[3];
    int count1 = countChips(pos1Docks);
    int count2 = countChips(pos2Docks);
    int pos1Copy[current_chips];
    int pos1CopyDocks[current_docks];
    int pos2Copy[current_chips];
    int pos2CopyDocks[current_docks];
    double sum_prob = 0;

    //initialize output and gValues_sub
    output[0] = 0;
    output[1] = 0;
    output[2] = 0;

    gValues_sub[0] = 0;
    gValues_sub[1] = 0;
    gValues_sub[2] = 0;

    //trivial cases
    //player 1 wins
    if (count1 == 0 && count2 > 0) {
        output[0] = 1;
        output[1] = 0;
        output[2] = 0;
    }
    //player 2 wins
    else if (count1 > 0 && count2 == 0) {
        output[0] = 0;
        output[1] = 1;
        output[2] = 0;
    }
    //tie game
    else if (count1 == 0 && count2 == 0) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    }
    //same positions
    else if (isSamePosition(pos1Docks, pos2Docks)) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    }
    //non-trivial case: more than one chip on one of the positions
    else {
        //get sum_prob
        for (int i = 0; i < current_docks; i++) {
            if (pos1Docks[i] > 0 || pos2Docks[i] > 0) {
                sum_prob += prob[i];
            }
        }

        //use recursion to add the next terms of the g Function.
        for (int i = 0; i < current_docks; i++) {
            if (pos1Docks[i] > 0 || pos2Docks[i] > 0) {
                copyPosition(pos1, pos1Docks, pos1Copy, pos1CopyDocks);
                copyPosition(pos2, pos2Docks, pos2Copy, pos2CopyDocks);

                removeChip(pos1Copy, pos1CopyDocks, i);
                removeChip(pos2Copy, pos2CopyDocks, i);

                gFunctionRecursive(pos1Copy, pos1CopyDocks, pos2Copy, pos2CopyDocks, prob, gValues_sub);

                //add to current value of gValues
                for (int j = 0; j < 3; j++) {
                    output[j] += (prob[i] / sum_prob) * gValues_sub[j];
                }
            }
        }
    }
}

//computes the probabilities of game outcomes using simulation.
void gFunctionSimulation(int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double output[]) {
    int diceRoll = 0;
    int count1 = 0;
    int count2 = 0;
    int pos1Copy[current_chips];
    int pos1CopyDocks[current_docks];
    int pos2Copy[current_chips];
    int pos2CopyDocks[current_docks];

    //initialize output
    output[0] = 0;
    output[1] = 0;
    output[2] = 0;

    //trivial case: same positions
    if (isSamePosition(pos1Docks, pos2Docks)) {
        output[0] = 0;
        output[1] = 0;
        output[2] = 1;
    }
    //non-trivial case: not same positions
    else {
        for (int i = 0; i < currentInterval; i++) {
            copyPosition(pos1, pos1Docks, pos1Copy, pos1CopyDocks);
            copyPosition(pos2, pos2Docks, pos2Copy, pos2CopyDocks);

            count1 = countChips(pos1CopyDocks);
            count2 = countChips(pos2CopyDocks);

            while (count1 > 0 && count2 > 0) {
                diceRoll = rollDice();

                removeChip(pos1Copy, pos1CopyDocks, diceRoll - 1);
                removeChip(pos2Copy, pos2CopyDocks, diceRoll - 1);

                count1 = countChips(pos1CopyDocks);
                count2 = countChips(pos2CopyDocks);
            }

            //game outcome
            if (count1 == 0 && count2 > 0) {
                output[0]++;
            }
            else if (count1 > 0 && count2 == 0) {
                output[1]++;
            }
            else if (count1 == 0 && count2 == 0) {
                output[2]++;
            }
        }

        for (int i = 0; i < 3; i++) {
            output[i] /= (double)currentInterval;
        }
    }
}

//initializes the arrays for the positions and docks.
void initializePositions(int pos1[], int docks1[], int pos2[], int docks2[], int pos3[], int docks3[], int pos4[], int docks4[], int dockOrder[]) {
    if (currentMode == MODE_GAME) { //game mode
        //clear all positions
        for (int i = 0; i < current_chips; i++) {
            pos1[i] = 0;
            pos2[i] = 0;
            pos3[i] = 0;
            pos4[i] = 0;
        }

        //clear all docks
        for (int i = 0; i < current_docks; i++) {
            docks1[i] = 0;
            docks2[i] = 0;
            docks3[i] = 0;
            docks4[i] = 0;
        }

        //clear dockOrder
        for (int i = 0; i < usable_docks; i++) {
            dockOrder[i] = 0;
        }
    }
    else if (currentAmount == AMOUNT_ONE) { //calculation mode: amount one
        //clear all positions
        for (int i = 0; i < current_chips; i++) {
            pos1[i] = 0;
            pos2[i] = 0;
            pos3[i] = 0;
            pos4[i] = 0;
        }

        //clear all docks
        for (int i = 0; i < current_docks; i++) {
            docks1[i] = 0;
            docks2[i] = 0;
            docks3[i] = 0;
            docks4[i] = 0;
        }
    }
    else { //calculation modes
        //pos1: leader position
        //pos2: mirror position
        //pos3: current position
        for (int i = 0; i < current_chips; i++) {
            addChip(pos1, docks1, dockOrder[0]);
            addChip(pos2, docks2, dockOrder[0]);
            addChip(pos3, docks3, dockOrder[0]);
        }

        //pos4: final position
        for (int i = 0; i < current_chips; i++) {
            addChip(pos4, docks4, dockOrder[usable_docks - 1]);
        }
    }
}

//allows the user to input the settings for computation and analysis.
void inputSettings() {
    char setting = 0;
    char accept = 'x';
    do {
        displayTitle();
            
        //input dice, faces, current chips
        printf("Input number of dice: ");
        scanf(" %d", &dice);
        printf("Input number of faces per die: ");
        scanf(" %d", &faces);
        current_docks = dice * faces;
        usable_docks = current_docks - dice + 1;
        //check for invalid input
        while (dice <= 0 || faces <= 0 || current_docks > MAX_CHIPS_DOCKS) {
            printf("Invalid dice information.\n");
            printf("Input number of dice: ");
            scanf(" %d", &dice);
            printf("Input number of faces per die: ");
            scanf(" %d", &faces);
            current_docks = dice * faces;
            usable_docks = current_docks - dice + 1;
        }
        printf("Input number of chips: ");
        scanf(" %d", &current_chips);
        //check for invalid input
        while (current_chips <= 0 || current_chips > MAX_CHIPS_DOCKS) {
            printf("Invalid number of chips.\n");
            printf("Input number of chips: ");
            scanf(" %d", &current_chips);
        }

        //input mode, amount, speed, interval
        //input mode
        printf("------------------------------\n");
        printf("Calculation and Analysis Modes\n");
        printf("[1] g Function: Recursive Method\n");
        printf("[2] g Function: Non-Recursive Method\n");
        printf("[3] g Function: Simulation Method\n");
        printf("[4] g Function: All Methods\n");
        printf("[5] Expected Duration: Recursive Method\n");
        printf("[6] Expected Duration: Simulation Method\n");
        printf("[7] Expected Duration: Both Methods\n");
        printf("Enter mode: ");
        scanf(" %c", &setting);
        currentMode = setting - '0';
        while (currentMode < 1 || currentMode > 7) {
            printf("Invalid mode. Enter mode: ");
            scanf(" %c", &setting);
            currentMode = setting - '0';
        }

        //input amount
        //only valid for non-all/both modes.
        if (currentMode != MODE_GALL && currentMode != MODE_EDBOTH) {
            printf("-----\n");
            printf("Amounts\n");
            printf("[1] One Position\n");
            printf("[2] All Positions\n");
            printf("Enter Amount: ");
            scanf(" %c", &setting);
            currentAmount = setting - '0';
            while (currentAmount < 1 || currentAmount > 2) {
                printf("Invalid amount. Enter amount: ");
                scanf(" %c", &setting);
                currentAmount = setting - '0';
            }
        }

        //input speed
        //only valid for all positions.
        if (currentAmount == AMOUNT_ALL) {
            printf("-----\n");
            printf("Speed\n");
            printf("[1] Speed 1: No positions skipped.\n");
            printf("[2] Speed 2: Positions with no chips in the middle skipped.\n");
            printf("[3] Speed 3: Positions with gaps are also skipped.\n");
            printf("[4] Speed 4: Positions which are not symmetric are also skipped.\n");
            printf("[5] Speed 5: Positions that don't have most chips in the middle are also skipped.\n");
            printf("[6] Speed 6: Positions are reduced to improve calculation time.\n");
            printf("Enter Speed: ");
            scanf(" %c", &setting);
            currentSpeed = setting - '0';
            while (currentSpeed < 1 || currentSpeed > 6) {
                printf("Invalid speed. Enter speed: ");
                scanf(" %c", &setting);
                currentSpeed = setting - '0';
            }
        }

        //input interval
        //only valid for simulation modes and all positions.
        if (currentAmount == AMOUNT_ALL || currentMode == MODE_GSIMULATION || currentMode == MODE_GALL || currentMode == MODE_EDSIMULATION || currentMode == MODE_EDBOTH) {
            printf("-----\n");
            printf("Interval\n");
            printf("[1] Extra Tiny: 1\n");
            printf("[2] Tiny: 10\n");
            printf("[3] Small: 100\n");
            printf("[4] Medium: 1000\n");
            printf("[5] Big: 10000\n");
            printf("[6] Huge: 100000\n");
            printf("[7] Massive: 1000000\n");
            printf("Enter Speed: ");
            scanf(" %c", &setting);
            int interval = setting - '0';
            while (interval < 1 || interval > 7) {
                printf("Invalid speed. Enter speed: ");
                scanf(" %c", &setting);
                interval = setting - '0';
            }
            //convert to currentInterval
            currentInterval = powInt(10, interval - 1);
        }

        //ask for settings approval
        printf("\n----------\n");
        printf("Press [y] to accept these settings. Other input will repeat the settings input. ");
        scanf(" %c", &accept);
    } while (accept != 'y');
}

//checks whether two positions are identical.
bool isSamePosition(int pos1Docks[], int pos2Docks[]) {
    bool isSame = true;

    for (int i = 0; i < current_docks; i++) {
        if (pos1Docks[i] != pos2Docks[i]) {
            isSame = false;
            break;
        }
    }

    return isSame;
}

//checks whether two roll combinations are identical.
bool isSameRollCombo(int combo1[], int combo2[]) {
    bool isSame = true;

    for (int i = 0; i < current_chips * 2; i++) {
        if (combo1[i] != combo2[i]) {
            isSame = false;
            break;
        }
    }

    return isSame;
}

//finds the dock with the most chips and returns the number of chips.
int maxDock(int docks[]) {
    int max = 0;

    for (int i = 0; i < current_docks; i++) {
        if (docks[i] > max) {
            max = docks[i];
        }
    }

    return max;
}

//finds the next position to help enumerate all possible initial positions.
void nextPosition(int pos[], int posDocks[], int dockOrder[]) {
    int idx1 = 0;
    int idx2 = 0;
    //rule 1: if last chip is not at last dockorder, move forward in dock order.
    if (pos[current_chips - 1] != dockOrder[usable_docks - 1] + 1) {
        //find last chip in dock order
        while (pos[current_chips - 1] != dockOrder[idx1] + 1) {
            idx1++;
        }
        //idx1 = index of last chip in dockorder
        //move forward in dock order
        pos[current_chips - 1] = dockOrder[idx1 + 1] + 1;
    }
    //rule 2: if last chip is at last dockorder, find the first chip from the end not at last dock order, and move it, along with all chips after, to the next dock order.
    else {
        idx2 = current_chips - 1;
        while (pos[idx2] == dockOrder[usable_docks - 1] + 1) {
            idx2--;
        }
        //idx2 = index of chip in pos that is not last in dockorder
        //find pos[idx2] in dock order
        idx1 = 0;
        while (pos[idx2] != dockOrder[idx1] + 1) {
            idx1++;
        }
        //idx1 = index of pos[idx2] in dock order
        //move forward pos[idx2] and succeeding chips in dockorder
        for (int i = idx2; i < current_chips; i++) {
            pos[idx2] = dockOrder[idx1 + 1] + 1;
        }
    }

    //update posDocks
    docks(pos, posDocks);
}

//finds the next roll combo to help enumerate all possible roll combinations to clear the board.
void nextRollCombo(int rollCombo[], int count) {
    int holder[count];
    int idx1 = count - 1;
    int idx2 = count - 1;

    //rule 1: if last two in ascending order, swap the two rolls.
    if (rollCombo[count - 1] > rollCombo[count - 2]) {
        holder[0] = rollCombo[count - 2];
        rollCombo[count - 2] = rollCombo[count - 1];
        rollCombo[count - 1] = holder[0];
    }
    //rule 2: if in descending order, swap roll before the descending order with
    //      the next higher roll. then sort the rest in ascending order.
    else {
        //find end of descending order
        do {
            idx1--;
        } while (idx1 > 0 && rollCombo[idx1] >= rollCombo[idx1 + 1]);
        //value of idx1: index of roll before descending order
        //find next higher roll
        while (idx2 > idx1 && rollCombo[idx2] <= rollCombo[idx1]) {
            idx2--;
        }
        //value of idx2: higher roll than rollCombo[idx1]
        //swap
        holder[idx2] = rollCombo[idx2];
        rollCombo[idx2] = rollCombo[idx1];
        rollCombo[idx1] = holder[idx2];
        //sort descending order into ascending order
        //copy into holder
        for (int i = idx1 + 1; i < count; i++) {
            holder[i] = rollCombo[i];
        }
        //copy from holder
        for (int i = count - 1; i > idx1; i--) {
            rollCombo[idx1 + count - i] = holder[i];
        }
    }
}

//plays an instance of the River Crossing Game. This assumes default settings of two dice with six faces.
void playGame(int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double prob[]) {
    char cmd = 'a';
    int count1 = countChips(pos1Docks);
    int count2 = countChips(pos2Docks);
    int dice1 = 0;
    int dice2 = 0;
    int rolls = 0;
    double ed1 = 0;
    double ed2 = 0;
    double gFunction[3];

    //play the game
    do {
        displayTitle();
        displayBoard(pos1Docks, pos2Docks);

        //initial prompt
        printf("---\nWelcome to the Game!\n");
        if (rolls > 0) {
            //output
            printf("The Dice rolled a %d with a %d and a %d!\n", dice1 + dice2, dice1, dice2);
        }
        if (cmd == 'e') {
            //output
            printf("Expected duration for Player 1: %lf\n", ed1);
            printf("Expected duration for Player 2: %lf\n", ed2);
            printf("Note: a value of -1 means an infinite expected duration.\n");
        }
        if (cmd == 'g') {
            //output
            printf("g Function Values: {%lf, %lf, %lf}\n", gFunction[0], gFunction[1], gFunction[2]);
            printf("This means that Player 1 has a %lf%% chance of winning, Player 2 has a %lf%% chance of winning, and a Tie Game has a %lf%% chance of occuring.\n", gFunction[0] * 100, gFunction[1] * 100, gFunction[2] * 100);
        }
        printf("Type [r] to roll the dice; type [e] for expected duration; type [g] for the g Function; type [x] to exit: ");
        scanf(" %c", &cmd);

        //check for invalid input
        while (cmd != 'r' && cmd != 'e' && cmd != 'g' && cmd != 'x') {
            printf("Invalid input. Your command: ");
            scanf(" %c", &cmd);
        }

        if (cmd == 'r') { //roll
            //roll the dice
            dice1 = (rand() % faces) + 1;
            dice2 = (rand() % faces) + 1;
            rolls++;

            //remove chip
            removeChip(pos1, pos1Docks, dice1 + dice2 - 1);
            removeChip(pos2, pos2Docks, dice1 + dice2 - 1);

            //count chips
            count1 = countChips(pos1Docks);
            count2 = countChips(pos2Docks);
        }
        else if (cmd == 'e') { //expected duration
            ed1 = expectedDurationRecursive(pos1, pos1Docks, prob);
            ed2 = expectedDurationRecursive(pos2, pos2Docks, prob);
        }
        else if (cmd == 'g') { //expected duration
            gFunctionRecursive(pos1, pos1Docks, pos2, pos2Docks, prob, gFunction);
        }
        else if (cmd == 'x') { //exit
            count1 = -1;
            count2 = -1;
        }
    } while (count1 > 0 && count2 > 0);

    displayTitle();
    displayBoard(pos1Docks, pos2Docks);
    //output the winner
    if (count1 == 0 & count2 > 0) {
        //player 1 wins
        printf("Player 1 Wins after %d rolls!\n", rolls);
    }
    else if (count1 > 0 && count2 == 0) {
        //player 2 wins
        printf("Player 2 Wins after %d rolls!\n", rolls);
    }
    else if (count1 == 0 && count2 == 0) {
        //tie game
        printf("Tie Game after %d rolls!\n", rolls);
    }
    else if (count1 == -1 && count2 == -1) {
        //game ended
        printf("Game ended after %d rolls!\n", rolls);
    }
}

//converts position (chip or dock format) into a string
void posToString(int pos[], char output[], int mode) {
    switch (mode) {
        case (STRING_CHIPS):
            for (int i = 0; i < current_chips; i++) {
                output[i] = DOCK_STRING[pos[i] - 1];
            }
            break;
        case (STRING_DOCKS):
            for (int i = 0; i < current_docks; i++) {
                if (pos[i] == 0) {
                    output[i] = '0';
                }
                else {
                    output[i] = DOCK_STRING[pos[i] - 1];
                }
            }
            break;
        default:
            printf("Invalid mode.\n");
    }
}

//integer version of pow(x, y) in math.h
int powInt(int x, int y) {
    int power = 1;
    //trivial cases
    if (y == 0 && x != 0) {
        power = 1;
    }
    else if (y == 1) {
        power = x;
    }
    //non-trivial case
    else if (y > 1) {
        power = x * (powInt(x, y - 1));
    }
    else {
        printf("undefined behaviour\n");
    }

    return power;
}

//prints the settings to console and later, to file.
void printSettings() {
    displayTitle();

    //print dice, faces, chips
    printf("Number of Dice: %d\n", dice);
    printf("Number of Faces: %d\n", faces);
    printf("Number of Chips: %d\n", current_chips);
    printf("-----\n");

    //print mode, amount, speed, interval
    //print mode
    switch (currentMode) {
        case (MODE_GRECURSIVE):
            printf("Mode: [1] g Function: Recursive Method\n");
            break;
        case (MODE_GNONRECURSIVE):
            printf("Mode: [2] g Function: Non-Recursive Method\n");
            break;
        case (MODE_GSIMULATION):
            printf("Mode: [3] g Function: Simulation Method\n");
            break;
        case (MODE_GALL):
            printf("Mode: [4] g Function: All Methods\n");
            break;
        case (MODE_EDRECURSIVE):
            printf("Mode: [5] Expected Duration: Recursive Method\n");
            break;
        case (MODE_EDSIMULATION):
            printf("Mode: [6] Expected Duration: Simulation Method\n");
            break;
        case (MODE_EDBOTH):
            printf("Mode: [7] Expected Duration: Both Methods\n");
            break;
        default:
            printf("Invalid mode.\n");
    }

    //print amount
    if (currentMode != MODE_GALL && currentMode != MODE_EDBOTH) {
        switch (currentAmount) {
            case (AMOUNT_ONE):
                printf("Amount: [1] One Position\n");
                break;
            case (AMOUNT_ALL):
                printf("Amount: [2] All Positions\n");
                break;
            default:
                printf("Invalid amount.\n");
        }
    }

    //print speed
    if (currentAmount == AMOUNT_ALL) {
        switch (currentSpeed) {
            case (SPEED_1):
                printf("Speed: [1] Speed 1: No positions skipped.\n");
                break;
            case (SPEED_2):
                printf("Speed: [2] Speed 2: Positions with no chips in the middle skipped.\n");
                break;
            case (SPEED_3):
                printf("Speed: [3] Speed 3: Positions with gaps are also skipped.\n");
                break;
            case (SPEED_4):
                printf("Speed: [4] Speed 4: Positions which are not symmetric are also skipped.\n");
                break;
            case (SPEED_5):
                printf("Speed: [5] Speed 5: Positions that don't have most chips in the middle are also skipped.\n");
                break;
            case (SPEED_6):
                printf("Speed: [6] Speed 6: Positions are reduced to improve calculation time.\n");
                break;
            default:
                printf("Invalid speed.\n");
        }
    }

    //print interval
    if (currentAmount == AMOUNT_ALL || currentMode == MODE_GSIMULATION || currentMode == MODE_GALL || currentMode == MODE_EDSIMULATION || currentMode == MODE_EDBOTH) {
        switch (currentInterval) {
            case (INTERVAL_EXTRATINY):
                printf("Interval: [1] Extra Tiny: 1\n");
                break;
            case (INTERVAL_TINY):
                printf("Interval: [2] Tiny: 10\n");
                break;
            case (INTERVAL_SMALL):
                printf("Interval: [3] Small: 100\n");
                break;
            case (INTERVAL_MEDIUM):
                printf("Interval: [4] Medium: 1000\n");
                break;
            case (INTERVAL_BIG):
                printf("Interval: [5] Big: 10000\n");
                break;
            case (INTERVAL_HUGE):
                printf("Interval: [6] Huge: 100000\n");
                break;
            case (INTERVAL_MASSIVE):
                printf("Interval: [7] Massive: 1000000\n");
            default:
                printf("Invalid interval.\n");
        }
    }
    printf("--------------------\n\n");
}

//removes a chip from a specific dock.
//dock counting starts at 0.
void removeChip(int pos[], int posDocks[], int dock) {
    //find first instance of dock+1 in pos
    for (int i = 0; i < current_chips; i++) {
        if (pos[i] == dock + 1) {
            for (int j = i; j < current_chips - 1; j++) {
                pos[j] = pos[j+1];
            }
            pos[current_chips - 1] = 0;
            break;
        }
    }

    //decrement dock in posDocks
    if (posDocks[dock] > 0) {
        posDocks[dock]--;
    }
}

//rolls the dice according to the dice and faces values and outputs the result.
int rollDice() {
    int diceSum = 0;

    for (int i = 0; i < dice; i++) {
        diceSum += (rand() % faces) + 1;
    }

    return diceSum;
}

//finds the probability of the roll combination rollCombo and adds it to the g probability values.
void rollComboIteration(int rollCombo[], int combinedDocks[], int rollCount, int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double prob[], double output[]) {
    double rollComboProb = 1;
    int roll = 0;
    double numerator;
    double denominator;
    int count1 = countChips(pos1Docks);
    int count2 = countChips(pos2Docks);

    //find probability
    for (int i = 0; i < rollCount; i++) {
        //numerator
        numerator = prob[rollCombo[i] - 1]; 
        //denominator
        denominator = 0;
        for (int j = 0; j < current_docks; j++) {
            if (combinedDocks[j] > 0) {
                denominator += prob[j];
            }
        }

        //multiply to current value
        if (i < rollCount - 1 || rollCombo[i] != 1) {
            rollComboProb *= (numerator / denominator);
        }

        //remove roll from combinedDocks
        if (combinedDocks[rollCombo[i] - 1] > 0) {
            combinedDocks[rollCombo[i] - 1]--;
        }
    }

    //check result
    do {
        removeChip(pos1, pos1Docks, rollCombo[roll] - 1);
        removeChip(pos2, pos2Docks, rollCombo[roll] - 1);
        roll++;

        count1 = countChips(pos1Docks);
        count2 = countChips(pos2Docks);
    } while (count1 > 0 && count2 > 0);

    //result
    if (count1 == 0 && count2 > 0) {
        //player 1 wins
        output[0] += rollComboProb;
    }
    else if (count1 > 0 && count2 == 0) {
        //player 2 wins
        output[1] += rollComboProb;
    }
    else if (count1 == 0 && count2 == 0) {
        //tie game
        output[2] += rollComboProb;
    }
}

//sets up the dock order for moving to the next position in the enumeration.
//dock counting starts at 0.
void setupDockOrder(int dockOrder[]) {
    //clear dockOrder
    for (int i = 0; i < usable_docks; i++) {
        dockOrder[i] = 0;
    }

    //fill up dockOrder with the docks.
    //start in the middle, then alternate left and right.
    if (usable_docks % 2 == 1) { //odd usable_docks
        dockOrder[0] = middle[0];

        //odd i: left, even i: right
        for (int i = 1; i < usable_docks; i++) {
            if (i % 2 == 1) { //left
                dockOrder[i] = middle[0] - ((i + 1) / 2);
            }
            else { //right
                dockOrder[i] = middle[0] + (i / 2);
            }
        }
    }
    else { //even usable docks
        //even i: left, odd i: right
        for (int i = 0; i < usable_docks; i++) {
            if (i % 2 == 0) { //left
                dockOrder[i] = middle[0] - (i / 2);
            }
            else { //right
                dockOrder[i] = middle[1] + ((i + 1) / 2);
            }
        }
    }
}

//sets up the probabilities array depending on the dice and faces values.
//faces is not changed, however numDice may change to use recursion.
void setupProbabilities(double prob[], int numDice) {
    double probCopy[current_docks];

    //initialize prob and probCopy
    //always size of current_docks
    for (int i = 0; i < current_docks; i++) {
        prob[i] = 0;
        probCopy[i] = 0;
    }

    //if one die, trivial case
    if (numDice == 1) {
        for (int i = 0; i < faces; i++) {
            prob[i] = 1 / (double)faces;
        }
    }
    //if more than one die, non-trivial case. use recursion
    else if (numDice > 1) {
        setupProbabilities(probCopy, numDice - 1);

        //process values
        for (int i = 1; i <= faces; i++) {
            for (int j = 0; j < faces * numDice; j++) {
                if (i + j < current_docks) {
                    prob[i + j] += probCopy[j];
                }
            }
        }

        for (int i = 0; i < current_docks; i++) {
            prob[i] /= faces;
        }
    }
}