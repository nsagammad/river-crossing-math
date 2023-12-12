import java.util.ArrayList;

public class Position {
    //character representations of numbers 1 through 35.
    private final String DOCK_STRING = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //arrayList of Docks.
    private ArrayList<Dock> docks = new ArrayList<Dock>();
    //number of chips in the position.
    private int count;

    //constructor
    public Position(int dice, int currentDocks) {
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
}