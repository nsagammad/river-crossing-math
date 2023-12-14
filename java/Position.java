import java.util.ArrayList;

public class Position {
    //character representations of numbers 1 through 35.
    private final String DOCK_STRING = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    //probabilities per dock.
    private static ArrayList<Double> probabilities = new ArrayList<Double>();

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

    //returns the probability of a certain dock.
    //dock counting starts at 0.
    public static double getProbability(int d) {
        if (!probabilities.isEmpty() && d < probabilities.size()) {
            return probabilities.get(d);
        }
        else {
            return 0;
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

    //sets up the arraylist of probabilities using dice d and faces f.
    public void setupProbabilities(int d, int f) {
        //throw exception if d or f < 1.
        if (d < 1 || f < 1) {
            throw new IllegalArgumentException("Dice and Faces should at least be 1");
        }
        //dice = 1: trivial case
        else if (d == 1) {
            probabilities.clear();
            for (int i = 0; i < f; i++) {
                probabilities.add(1 / (double)f);
            }
        }
        //non-trivial case: more than 1 die
        else if (d > 1) {
            //use recursion
            setupProbabilities(d - 1, f);

            //copy over previous values
            ArrayList<Double> probCopy = new ArrayList<Double>();
            for (int i = 0; i < probabilities.size(); i++) {
                probCopy.add(probabilities.get(i));
            }

            //clear probabilities
            probabilities.clear();

            //add the values of probcopy
            
        }
    }
}