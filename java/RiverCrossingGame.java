import java.util.Scanner;

public class RiverCrossingGame {
  //constants
  private static final int MAX_CHIPSDOCKS = 35;

  //objects
  static Scanner inputScanner = new Scanner(System.in);

  //variables
  static int dice = 0;
  static int faces = 0;
  static int currentDocks = 0;

  //functions
  public static void inputDiceFaces() {
    do {
      if (currentDocks > MAX_CHIPSDOCKS) {
        System.out.println("Invalid number of dice and faces. Please consider lowering the number of dice or faces.");
      }
      System.out.println("Enter number of dice: ");
      try {
        dice = inputScanner.nextInt();
      }
      catch (Exception e) {
        System.out.println("Your input was not a valid number. Default number of 2 dice assigned.");
        dice = 2;
      }
      System.out.println("Enter number of faces per die: ");
      try {
        faces = inputScanner.nextInt();
      }
      catch (Exception e) {
        System.out.println("Your input was not a valid number. Default 6 faces per die assigned.");
        faces = 6;
      }
      currentDocks = dice * faces;
    } while (currentDocks > MAX_CHIPSDOCKS);
  }

  public static void main(String[] args) {
    System.out.println("Hello World");
    inputDiceFaces();

    Position Pos1 = new Position(dice, currentDocks);
    System.out.println(Pos1.docksToString());
  }
}