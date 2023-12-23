import java.util.Scanner;
import java.util.Random;
import java.math.BigInteger;
import java.math.BigDecimal;
import java.math.MathContext;

public class Game {
    //game variables
    private static int dice = 2;
    private static int faces = 6;
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
    private static Random random = new Random();
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
        BigInteger ed1[] = {BigInteger.ZERO, BigInteger.ZERO};
        BigInteger ed2[] = {BigInteger.ZERO, BigInteger.ZERO};
        BigDecimal edholder[] = {BigDecimal.ONE, BigDecimal.ONE};
        double edsim1[] = {0d, 0d};
        double edsim2[] = {0d, 0d};
        BigInteger gfunc[][] = {{BigInteger.ZERO, BigInteger.ONE}, {BigInteger.ZERO, BigInteger.ONE}, {BigInteger.ZERO, BigInteger.ONE}};
        BigDecimal gholder[] = {BigDecimal.ONE, BigDecimal.ONE, BigDecimal.ONE};

        System.out.println("-----");
        displayBoard(position1, position2);
        System.out.println("Let's Play!");

        while (count1 > 0 && count2 > 0) {
            //check for invalid input
            do {
                if (!gameInput.equals("R") && !gameInput.equals("E") && !gameInput.equals("X") && !gameInput.equals("G")) {
                    System.out.println("Invalid Input.");
                }
                if (rolls >= 1 && gameInput.equals("R")) {
                    System.out.println("Dice rolled a " + diceRoll + "!");
                    System.out.println(removed1 + " chips removed from Player 1.");
                    System.out.println(removed2 + " chips removed from Player 2.");
                }
                else if (gameInput.equals("E")) {
                    System.out.println("Expected Duration for Player 1 (Recursion): " + edholder[0].toString());
                    System.out.println("    Fractional Equivalent: " + ed1[0].toString() + "/" + ed1[1].toString());
                    if (edsim1[1] != 0d) {
                        System.out.println("Expected Duration for Player 1 (Simulation): " + (edsim1[0] / edsim1[1]));
                    }
                    else {
                        System.out.println("Expected Duration for Player 1 (Simulation): Infinite");
                    }
                    System.out.println("-----");
                    System.out.println("Expected Duration for Player 2 (Recursion): " + edholder[1].toString());
                    System.out.println("    Fractional Equivalent: " + ed2[0].toString() + "/" + ed2[1].toString());
                    if (edsim2[1] != 0d) {
                        System.out.println("Expected Duration for Player 1 (Simulation): " + (edsim2[0] / edsim2[1]));
                    }
                    else {
                        System.out.println("Expected Duration for Player 1 (Simulation): Infinite");
                    }
                }
                else if (gameInput.equals("G")) {
                    System.out.println("g(A, B) = {" + gholder[0].toString() + ", " + gholder[1].toString() + ", " + gholder[2].toString() + "}");
                    System.out.println("Fractional equivalents:");
                    System.out.println("p(A wins) = " + gfunc[0][0] + "/" + gfunc[0][1]);
                    System.out.println("p(B wins) = " + gfunc[1][0] + "/" + gfunc[1][1]);
                    System.out.println("p(Tie Game) = " + gfunc[2][0] + "/" + gfunc[2][1]);
                }
                System.out.println("Type [R] to roll the dice!");
                gameInput = inputScanner.nextLine();
                gameInput = gameInput.toUpperCase();
                gameInput = gameInput.substring(0, 1);
            } while (!gameInput.equals("R") && !gameInput.equals("E") && !gameInput.equals("X") && !gameInput.equals("G"));

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
            else if (gameInput.equals("E")) {
                ed1 = position1.expectedDurationRecursive();
                edholder[0] = new BigDecimal(ed1[0]).divide(new BigDecimal(ed1[1]), MathContext.DECIMAL64);
                ed2 = position2.expectedDurationRecursive();
                edholder[1] = new BigDecimal(ed2[0]).divide(new BigDecimal(ed2[1]), MathContext.DECIMAL64);
                edsim1 = position1.expectedDurationSimulation(10000);
                edsim2 = position2.expectedDurationSimulation(10000);
            }
            //input G: g function
            else if (gameInput.equals("G")) {
                gfunc = position1.gFunctionRecursive(position2);
                gholder[0] = new BigDecimal(gfunc[0][0]).divide(new BigDecimal(gfunc[0][1]), MathContext.DECIMAL64);
                gholder[1] = new BigDecimal(gfunc[1][0]).divide(new BigDecimal(gfunc[1][1]), MathContext.DECIMAL64);
                gholder[2] = new BigDecimal(gfunc[2][0]).divide(new BigDecimal(gfunc[2][1]), MathContext.DECIMAL64);
            }
            //input X: exit game
            else if (gameInput.equals("X")) {
                System.out.println("Exiting...");
                break;
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

        if (gameInput.equals("X")) {
            System.out.println("Game exited after " + rolls + " rolls.");
        }
    }

    public static int rollDice() {
        int roll = 0;

        for (int i = 0; i < dice; i++) {
            roll += random.nextInt(faces) + 1;
        }

        return roll;
    }

    public static void setDice(int d, int f) {
        dice = d;
        faces = f;
    }
}
