import java.util.Scanner;
import java.util.Random;
import java.math.BigDecimal;
import java.math.MathContext;

public class Game {
    //game variables
    private int dice;
    private int faces;
    private int chips;
    private int docks;

    //position variables
    private Position position1;
    private Position position2;
    private Position emptyPos;

    //input variables
    private Scanner inputScanner = new Scanner(System.in);
    private String positionInput;
    private String[] splitInput;
    private String gameInput = "R";

    //random variables
    private Random random = new Random();
    private int randomDock = 0;
    private int diceRoll = 0;
    private int rolls = 0;
    private int removed1 = 0;
    private int removed2 = 0;

    //constructor for the Game object.
    public Game(int d, int f, int c) {
        dice = d;
        faces = f;
        chips = c;
        docks = Position.getDocks();

        position1 = new Position(dice);
        position2 = new Position(dice);
        emptyPos = new Position(dice);
    }

    //displays the current board state.
    public void displayBoard(Position pos1, Position pos2) {
        int max1 = pos1.maxDockChips();
        int max2 = pos2.maxDockChips();

        //upper docks
        for (int i = 0; i < chips; i++) {
            if (chips - i <= max1) {
                System.out.print("|");
            }
            for (int j = 0; j < docks; j++) {
                if (chips - i <= pos1.getChips(j)) {
                    System.out.print(" (O) |");
                }
                else if (chips - i <= max1) {
                    System.out.print("     |");
                }
            }
            if (chips - i <= max1) {
                System.out.println("");
            }
        }

        // dock numbers and river
        System.out.print("~");
        for (int i = 0; i < docks; i++) {
            System.out.print("~~~~~~");
        }
        System.out.println("");
        System.out.print("|");
        for (int i = 0; i < docks; i++) {
            System.out.print("  " + Position.DOCK_STRING.substring(i, i + 1) + "  |");
        }
        System.out.println("");
        System.out.print("~");
        for (int i = 0; i < docks; i++) {
            System.out.print("~~~~~~");
        }
        System.out.println("");

        //lower docks
        for (int i = 0; i < chips; i++) {
            if (i + 1 <= max2) {
                System.out.print("|");
            }
            for (int j = 0; j < docks; j++) {
                if (i + 1 <= pos2.getChips(j)) {
                    System.out.print(" (O) |");
                }
                else if (i + 1 <= max2) {
                    System.out.print("     |");
                }
            }
            if (i + 1 <= max2) {
                System.out.println("");
            }
        }
    }

    //allows the user to input the two positions for the game.
    public void inputPositions() {
        //Player 1
        System.out.println("-----");
        displayBoard(position1, emptyPos);
        System.out.println("Player 1");
        System.out.println("Type out the docks you want to put chips in with commas in between.");
        System.out.println("Example: 4, 4, 4, 5, 5, 6, 7, 8, 9, A");
        System.out.println("NOTE: If some or all of the input is invalid, remaining chips will be placed randomly on the board.");
        System.out.print("Your position: ");
        try {
            positionInput = inputScanner.nextLine();
        }
        catch (Exception e) {
            System.out.println("Invalid input. Exiting program.");
        }
        //process input for player 1
        splitInput = positionInput.split("[, ]+");
        for (String s : splitInput) {
            s = s.toUpperCase();
            //s needs to be a single character and be inside dock_string. chip count must not exceed 12.
            if (Position.DOCK_STRING.contains(s) && s.length() == 1 && position1.getCount() < chips) { 
                position1.addChip(Position.DOCK_STRING.indexOf(s), 1);
            }
        }
        //if chip count of position 1 is less than chips (12), randomly add remaining chips to the board.
        while (position1.getCount() < chips) {
            randomDock = random.nextInt(docks);
            position1.addChip(randomDock, 1);
            System.out.println("Chip randomly added to dock " + (randomDock + 1) + ".");
        }

        //Player 2
        System.out.println("-----");
        displayBoard(emptyPos, position2);
        System.out.println("Player 2");
        System.out.println("Type out the docks you want to put chips in with commas in between.");
        System.out.println("Example: 4, 4, 4, 5, 5, 6, 7, 8, 9, A");
        System.out.println("NOTE: If some or all of the input is invalid, remaining chips will be placed randomly on the board.");
        System.out.print("Your position: ");
        try {
            positionInput = inputScanner.nextLine();
        }
        catch (Exception e) {
            System.out.println("Invalid input. Exiting program.");
        }
        //process input for player 2
        splitInput = positionInput.split("[, ]+");
        for (String s : splitInput) {
            s = s.toUpperCase();
            //s needs to be a single character and be inside dock_string. chip count must not exceed 12.
            if (Position.DOCK_STRING.contains(s) && s.length() == 1 && position2.getCount() < chips) { 
                position2.addChip(Position.DOCK_STRING.indexOf(s), 1);
            }
        }
        //if chip count of position 1 is less than chips (12), randomly add remaining chips to the board.
        while (position2.getCount() < chips) {
            randomDock = random.nextInt(docks);
            position2.addChip(randomDock, 1);
            System.out.println("Chip randomly added to dock " + (randomDock + 1) + ".");
        }
    }

    public void playGame() {
        int count1 = position1.getCount();
        int count2 = position2.getCount();
        BigDecimal ed1[] = {BigDecimal.ZERO, BigDecimal.ZERO};
        BigDecimal ed2[] = {BigDecimal.ZERO, BigDecimal.ZERO};

        System.out.println("-----");
        displayBoard(position1, position2);
        System.out.println("Let's Play!");

        while (count1 > 0 && count2 > 0) {
            //check for invalid input
            do {
                if (!gameInput.equals("R") && !gameInput.equals("E")) {
                    System.out.println("Invalid Input.");
                }
                if (rolls >= 1 && gameInput.equals("R")) {
                    System.out.println("Dice rolled a " + diceRoll + "!");
                    System.out.println(removed1 + " chips removed from Player 1.");
                    System.out.println(removed2 + " chips removed from Player 2.");
                }
                else if (gameInput.equals("E")) {
                    System.out.println("Expected Duration for Player 1: " + ed1[0].divide(ed1[1], MathContext.DECIMAL128).toString());
                    System.out.println("Expected Duration for Player 2: " + ed2[0].divide(ed2[1], MathContext.DECIMAL128).toString());
                }
                System.out.println("Type [R] to roll the dice!");
                gameInput = inputScanner.nextLine();
                gameInput = gameInput.toUpperCase();
                gameInput = gameInput.substring(0, 1);
            } while (!gameInput.equals("R") && !gameInput.equals("E"));

            //once we have the R, roll the dice
            if (gameInput.equals("R")) {
                diceRoll = rollDice();
                //remove chips
                removed1 = position1.removeChip(diceRoll - 1, 1);
                count1 = position1.getCount();
                removed2 = position2.removeChip(diceRoll - 1, 1);
                count2 = position2.getCount();
                rolls++;
            }
            //input E: expected duration
            if (gameInput.equals("E")) {
                ed1 = position1.expectedDurationRecursive();
                ed2 = position2.expectedDurationRecursive();
            }

            System.out.println("-----");
            displayBoard(position1, position2);
        }

        //game result
        if (count1 == 0 && count2 > 0) {
            System.out.println("Player 1 wins after " + rolls + " rolls!");
        }
        else if (count1 > 0 && count2 == 0) {
            System.out.println("Player 2 wins after " + rolls + " rolls!");
        }
        else if (count1 == 0 && count2 == 0) {
            System.out.println("Tie game after " + rolls + " rolls!");
        }
    }

    private int rollDice() {
        int roll = 0;

        for (int i = 0; i < dice; i++) {
            roll += random.nextInt(faces) + 1;
        }

        return roll;
    }
}
