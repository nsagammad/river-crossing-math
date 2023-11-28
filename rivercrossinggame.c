/* This program allows the user to play an instance of the River Crossing game.
    Besides this, this program allows the user to find the Expected Duration of an
    initial position, or find the probability of the game outcome of two 
    initial positions.

    Author: Nikko Gammad
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

//function declarations
void addChip(int[], int[], int);
void copyPosition(int[], int[], int[], int[]);
int countChips(int[]);
void displayBoard(int[], int[]);
void displayTitle();
double expectedDurationRecursive(int[], int[], double[]);
void gameInputChips(int[], int[]);
void initializePositions(int[], int[], int[], int[], int[], int[], int[], int[]);
int maxDock(int[]);
void playGame(int[], int[], int[], int[], double[]);
void removeChip(int[], int[], int);
void setupProbabilities(double[], int);

//constants
const int MAX_CHIPS_DOCKS = 25;
const char DOCK_STRING[25] = "123456789ABCDEFGHIJKLMNOP"; 
const int MODE_GAME = 0;

//global variables
int dice = 2;
int faces = 6;
int current_chips = 2;
int current_docks = 12;
int currentPlayer = 0; //0: player 1; 1: player 2
int currentMode = 0;

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
            if (currentPlayer == 0) {
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
        printf(" %2d  |", i + 1);
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
    printf("-------------------\n");
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

//lets the user add chips to the board.
//dock counting starts at 0.
void gameInputChips(int pos[], int posDocks[]) {
    int inputDock = 0;

    //ask for input
    printf("Player %d:\n", currentPlayer + 1);
    printf("On which dock would you like your chip to be placed? ");
    scanf(" %d", &inputDock);
    inputDock--;

    //check for invalid input
    while (inputDock < 0 || inputDock >= current_docks) {
        printf("Invalid dock number.\n");
        printf("On which dock would you like your chip to be placed? ");
        scanf(" %d", &inputDock);
        inputDock--;
    }

    //add chips
    addChip(pos, posDocks, inputDock);

    //switch player
    if (currentPlayer == 0) {
        currentPlayer = 1;
    }
    else {
        currentPlayer = 0;
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

void playGame(int pos1[], int pos1Docks[], int pos2[], int pos2Docks[], double prob[]) {
    char cmd = 'a';
    int count1 = countChips(pos1Docks);
    int count2 = countChips(pos2Docks);
    int dice1 = 0;
    int dice2 = 0;
    int rolls = 0;
    double ed1 = 0;
    double ed2 = 0;

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
        printf("Type [r] to roll the dice; type [e] for expected duration; type [x] to exit: ");
        scanf(" %c", &cmd);

        //check for invalid input
        while (cmd != 'r' && cmd != 'e' && cmd != 'x') {
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