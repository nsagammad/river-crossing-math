import java.util.ArrayList;

public class Dock {
    private int chips;
    private boolean usable;

    //probabilities per dock.
    private static ArrayList<Double> probabilities = new ArrayList<Double>();

    //constructor with only useValue
    public Dock(boolean useValue) {
        chips = 0;
        setUsable(useValue);
    }

    //constructor with number of chips
    public Dock(int chipNum, boolean useValue) {
        chips = 0;
        addChip(chipNum);
        setUsable(useValue);
    }
    
    //adds chips to the dock.
    public void addChip(int num) {
        chips += num;
    }

    //returns the number of chips on the dock.
    public int chipCount() {
        return chips;
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

    //returns the value of usable. true if the dock number can be rolled.
    public boolean isUsable() {
        return usable;
    }

    //removes chips from a dock. returns the number of chips removed.
    public int removeChip(int num) {
        int removed;
        if (chips >= num) {
            chips -= num;
            removed = num;
        }
        else {
            removed = chips;
            chips = 0;
        }
        
        return removed;
    }

    //sets up the arraylist of probabilities using dice d and faces f.
    public static void setupProbabilities(int d, int f) {
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
            //add 0 to first index
            probabilities.add(0.0);

            //add the values of probcopy
            for (int i = 1; i <= f; i++) { //iterate from 1 to f
                for (int j = 0; j < (d - 1) * f; j++) {
                    if (i + j == probabilities.size()) {
                        //add new element
                        probabilities.add(probCopy.get(j));
                    }
                    else {
                        //add to existing element
                        probabilities.set(i + j, probabilities.get(i + j) + probCopy.get(j));
                    }
                }
            }
            
            //divide by faces
            for (int i = 0; i < d * f; i++) {
                probabilities.set(i, probabilities.get(i) / f);
            }
        }
    }

    //sets the value of usable, i.e. if the dock number can be rolled.
    public void setUsable(boolean useValue) {
        usable = useValue;
    }
}