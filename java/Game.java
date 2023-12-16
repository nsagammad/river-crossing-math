import java.util.Scanner;
import java.util.Random;

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

    //random variables
    private Random random = new Random();
    private int randomDock = 0;

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
            if (Position.DOCK_STRING.indexOf(s) > 0 && s.length() == 1 && position1.getCount() < chips) { 
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
            if (Position.DOCK_STRING.indexOf(s) >= 0 && s.length() == 1 && position2.getCount() < chips) { 
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
        System.out.println("-----");
        displayBoard(position1, position2);
    }
}
