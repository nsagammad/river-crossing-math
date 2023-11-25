/* This program allows the user to play an instance of the River Crossing game.
    Besides this, this program allows the user to find the Expected Duration of an
    initial position, or find the probability of the game outcome of two 
    initial positions.

    Author: Nikko Gammad
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void displayBoard();
void displayTitle();

//global variables
int dice = 2;
int faces = 6;
int current_chips = 12;
int current_docks = 12;

int main() {
    //local variables
    char choice1 = '3'; //choice between playing the game or doing the calculations.
    current_docks = dice * faces;
    int position1[current_chips];
    int position1docks[current_docks];
    int position2[current_chips];
    int position2docks[current_docks];

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
    while (choice1 != '1' && choice1 != '2' && choice1 != 'g') {
        printf("Invalid input. Your choice: ");
        scanf(" %c", &choice1);
    }

    //play the original game
    if (choice1 == '1') {
        system("cls");
        displayTitle();
        displayBoard();
    }
    //computation and analyses
    else if (choice1 == '2') {
        system("cls");
        displayTitle();
        printf("Coming soon...\n");
    }
    //function testing
    else if (choice1 == 'g') {

    }
    return 0;
}

void displayBoard() {
    //upper docks
    for (int i = 0; i < 5; i++) {
        printf("|");
        for (int j = 0; j < current_docks; j++) {
            printf("     |");
        }
        printf("\n");
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
    for (int i = 0; i < 5; i++) {
        printf("|");
        for (int j = 0; j < current_docks; j++) {
            printf("     |");
        }
        printf("\n");
    }
}

void displayTitle() {
    printf("-------------------\n");
    printf("River Crossing Game\n");
    printf("-------------------\n\n");
}