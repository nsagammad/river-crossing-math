import java.util.ArrayList;

public class Position {
    //character representations of numbers 1 through 35.
    //character representations of numbers 1 through 35.
    private final String DOCK_STRING = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //arrayList of Docks.
    //probabilities per dock.
    private static ArrayList<Double> probabilities = new ArrayList<Double>();

    //arrayList of Docks.
    private ArrayList<Dock> docks = new ArrayList<Dock>();
    //number of chips in the position.
    private int count;

    //middle of the position.
    private static int[] middle = {0, 0};
    //current number of docks.
    private static int currentDocks = 12;
    //current number of usable docks.
    private static int usableDocks = 11;

    //constructor
    public Position(int dice) {
        for (int i = 0; i < currentDocks; i++) {
            Dock d = new Dock(false);
            if (i >= dice - 1) {
                d.setUsable(true);
            }
            else {
                d.setUsable(false);
            }
            docks.add(d);
        }
        count = 0;
    }

    //adds num number of chips to dock d.
    //dock counting starts at 0.
    public void addChip(int d, int num) {
        if (d < docks.size()) {
            docks.get(d).addChip(num);
            count += num;
        }
        else {
            throw new ArrayIndexOutOfBoundsException("No dock with that number in docks");
        }
    }

    //returns the number of chips in the position.
    public int getCount() {
        return count;
    }

    //returns a string representing the number of chips in each dock.
    public String docksToString() {
        String chipsStr = "";

        for (Dock d : docks) {
            if (d.chipCount() == 0) {
                chipsStr = chipsStr.concat("0");
            }
            else {
                chipsStr = chipsStr.concat(DOCK_STRING.substring(d.chipCount() - 1, d.chipCount()));
            }
        }
        
        return chipsStr;
    }

    //sets the middle dock numbers based on d(dice).
    //dock counting starts at 0.
    public void findMiddle(int d) {
        //assume that usableDocks has already been set
        if (usableDocks % 2 == 1) { //odd
            middle[0] = ((currentDocks + d) / 2) - 1;
            middle[1] = middle[0];
        }
        else { //even
            middle[0] = ((currentDocks + d - 1) / 2) - 1;
            middle[1] = ((currentDocks + d + 1) / 2) - 1;
        }
    }

    //removes num number of chips from dock d.
    //dock counting starts at 0.
    public void removeChip(int d, int num) {
        if (d < docks.size()) {
            int removed = docks.get(d).removeChip(num);
            count -= removed;
        }
        else {
            throw new ArrayIndexOutOfBoundsException("No dock with that number in docks");
        }
    }

    //sets the value of currentDocks and usableDocks.
    public static void setDocks(int docks, int dice) {
        currentDocks = docks;
        usableDocks = currentDocks - dice + 1;
    }
}