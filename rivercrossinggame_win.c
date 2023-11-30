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
void buildRollCombo(int[], int[], int[], int[]);
void copyPosition(int[], int[], int[], int[]);
int countChips(int[]);
void displayBoard(int[], int[]);
void displayTitle();
double expectedDurationRecursive(int[], int[], double[]);
double expectedDurationSimulation(int[], int[]);
void gameInputChips(int[], int[]);
void gFunctionNonRecursive(int[], int[], int[], int[], double[], double[]);
void gFunctionRecursive(int[], int[], int[], int[], double[], double[]);
void gFunctionSimulation(int[], int[], int[], int[], double[]);
void initializePositions(int[], int[], int[], int[], int[], int[], int[], int[]);
bool isSamePosition(int[], int[]);
int maxDock(int[]);
void nextRollCombo(int[]);
void playGame(int[], int[], int[], int[], double[]);
void removeChip(int[], int[], int);
int rollDice();
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
    MODE_EDRECURSIVE, //find best expected duration using recursive ed Function
    MODE_EDSIMULATION //using simulation
};
enum Speed {
    SPEED_1, //no skips
    SPEED_2, //only with chips in the middle
    SPEED_3, //no gaps
    SPEED_4, //symmetric
    SPEED_5, //most of chips in the middle
    SPEED_6 //reduced positions
};
enum Interval { //used for number of simulations and for intervals between console updates when computing for all positions.
    INTERVAL_EXTRATINY = 1,
    INTERVAL_TINY = 10,
    INTERVAL_SMALL = 100,
    INTERVAL_MEDIUM = 1000,
    INTERVAL_BIG = 10000,
    INTERVAL_HUGE = 100000,
    iNTERVAL_MASSIVE = 1000000   
};

//global variables
int dice = 2;
int faces = 6;
int current_chips = 4;
int current_docks = 12;
enum Player currentPlayer = PLAYER_1;
enum Mode currentMode = MODE_GAME;
enum Speed currentSpeed = SPEED_1;
enum Interval currentInterval = INTERVAL_MEDIUM;

int main() {
    //local variables
    char choice1 = '3'; //choice between playing the game or doing the calculations.
    current_docks = dice * faces;

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

        initializePositions(pos1, pos1Docks, pos2, pos2Docks, pos1Copy, pos1CopyDocks, pos2Copy, pos2CopyDocks);
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
        }
        //final board, ready to play the game
        playGame(pos1, pos1Docks, pos2, pos2Docks, probabilities);
    }
    //computation and analyses
    else if (choice1 == '2') {
        displayTitle();
        printf("Coming soon...\n");

        //input dice, faces, current chips

        //initialize these after input.
        int leaderPos[current_chips]; //this holds the data for the leader position.
        int leaderDocks[current_docks];
        int mirrorPos[current_chips]; //holds the data for a position which mirrors the leader.
        int mirrorDocks[current_docks];
        int position[current_chips]; //this holds the data for the current position being checked.
        int positionDocks[current_docks];
        int finalPos[current_chips]; //this holds the data for the final position.
        int finalDocks[current_docks];

        initializePositions(leaderPos, leaderDocks, mirrorPos, mirrorDocks, position, positionDocks, finalPos, finalDocks);
    }
    //function testing
    else if (choice1 == 'g') {
        printf("Welcome to function testing mode\n");
        printf("Here is where I place all my function tests.\n");
        //remember to remove this in the final version

        current_docks = 18;
        double prob[current_docks];
        setupProbabilities(prob, 3);
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
void buildRollCombo(int pos1Docks[], int pos2Docks[], int output1[], int output2[]) {
    int idx = 0;
    
    //clear output
    for (int i = 0; i < current_chips * 2; i++) {
        output1[i] = 0;
        output2[i] = 0;
    }

    //fill up rollCombo with chips to be cleared.
    for (int i = 0; i < current_docks; i++) {
        if (pos1Docks[i] > 0 || pos2Docks[i] > 0) {
            if (pos1Docks[i] >= pos2Docks[i]) { //player 1 has more or equal chips
                for (int j = 0; j < pos1Docks[i]; j++) {
                    output1[idx] = i + 1;
                    idx++;
                }
            }
            else { //player 2 has more chips
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

    ed = (double)rolls / currentInterval;

    return ed;
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

    //switch player
    if (currentPlayer == PLAYER_1) {
        currentPlayer = PLAYER_2;
    }
    else {
        currentPlayer = PLAYER_1;
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
        buildRollCombo(pos1Docks, pos2Docks, rollCombo, finalCombo);
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
void initializePositions(int pos1[], int docks1[], int pos2[], int docks2[], int pos3[], int docks3[], int pos4[], int docks4[]) {
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
    }
    else { //calculation modes
        printf("TODO: Initialize positions for calculation modes\n");
    }
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

//finds the next roll combo to help enumerate all possible roll combinations to clear the board.
void nextRollCombo(int rollCombo[]) {

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