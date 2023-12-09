public class Dock {
    private int chips;
    private double probability;
    private boolean usable;

    //constructor with only useValue
    public Dock(boolean useValue) {
        chips = 0;
        probability = 0;
        setUsable(useValue);
    }

    //constructor without number of chips
    public Dock(double prob, boolean useValue) {
        chips = 0;
        setProb(prob);
        setUsable(useValue);
    }

    //constructor with number of chips
    public Dock(int chipNum, double prob, boolean useValue) {
        chips = 0;
        addChip(chipNum);
        setProb(prob);
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

    //returns the probability of the dock number being rolled.
    public double getProb() {
        return probability;
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

    //sets the value of probability of rolling the number of the dock.
    public void setProb(double newProb) {
        probability = newProb;
    }

    //sets the value of usable, i.e. if the dock number can be rolled.
    public void setUsable(boolean useValue) {
        usable = useValue;
    }
}