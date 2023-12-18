import java.util.Scanner;

public class RiverCrossingGame {
  //constants
  private static final int MAX_CHIPSDOCKS = 35;

  //objects
  static Scanner inputScanner = new Scanner(System.in);

  //variables
  static int dice = 2;
  static int faces = 6;
  static int currentChips = 12;
  static int docks = 12;

  //functions
  //displays the title
  public static void displayTitle() {
    System.out.println("-----------------------");
    System.out.println("The River Crossing Game");
    System.out.println("-----------------------");
  }

  //allows the user to input the number of dice and number of faces in each die.
  public static void inputDiceFaces() {
    do {
      if (docks > MAX_CHIPSDOCKS) {
        System.out.println("Invalid number of dice and faces. Please consider lowering the number of dice or faces.");
      }
      System.out.print("Enter number of dice: ");
      try {
        dice = inputScanner.nextInt();
      }
      catch (Exception e) {
        System.out.println("Your input was not a valid number. Default number of 2 dice assigned.");
        dice = 2;
      }
      System.out.print("Enter number of faces per die: ");
      try {
        faces = inputScanner.nextInt();
      }
      catch (Exception e) {
        System.out.println("Your input was not a valid number. Default 6 faces per die assigned.");
        faces = 6;
      }
      docks = dice * faces;
    } while (docks > MAX_CHIPSDOCKS);

    //after input, numbers are valid
    Position.setDocks(docks, dice);
  }

  public static int inputProgramMode() {
    //initialize
    int choice = 1;

    do {
      //1: original game, 2: analyses, 3: roundrobin, 4: exit
      if (choice < 1 || choice > 4) {
        System.out.println("Invalid input.");
      }
      System.out.println("Type [1] to play the original game.");
      System.out.println("Type [2] for the computations and analyses.");
      System.out.print("Your choice: ");
      try {
        choice = inputScanner.nextInt();
      }
      catch (Exception e) {
        System.out.println("Invalid input. Exiting program.");
      }
    } while (choice < 1 || choice > 4);

    return choice;
  }

  public static void main(String[] args) {
    displayTitle();
    
    int choice1 = inputProgramMode();

    if (choice1 == 1) {
      System.out.println("Prepare to play the game.");
      dice = 2;
      faces = 6;
      currentChips = 4;
      docks = dice * faces;
      Position.setDocks(docks, dice);
      Dock.setupProbabilities(dice, faces);
      Game g = new Game(dice, faces, currentChips);
      g.inputPositions();
      g.playGame();
    }
    else if (choice1 == 2) {
      System.out.println("Computations and Analyses.");
      Dock.setupProbabilities(2, 6);
      System.out.println(Dock.getProbability(6).toString());
    }
    else if (choice1 == 3) {
      System.out.println("Round Robin Results.");
    }
    else if (choice1 == 4) {
      System.out.println("Exiting Program.");
    }
  }
}