import java.util.ArrayList;
import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;

public class Dock {
    private int chips;
    private boolean usable;

    //probabilities per dock.
    private static ArrayList<BigInteger> probNumerator = new ArrayList<BigInteger>();
    private static BigInteger probDenominator = BigInteger.ONE;

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
    public static BigDecimal getProbability(int d) {
        if (!probNumerator.isEmpty() && d < probNumerator.size()) {
            BigDecimal num = new BigDecimal(probNumerator.get(d));
            BigDecimal den = new BigDecimal(probDenominator);
            return num.divide(den, MathContext.DECIMAL128);
        }   
        else {
            return BigDecimal.ZERO;
        }
    }

    //returns the probability denominator.
    public static BigInteger getProbabilityDenominator() {
        return probDenominator;
    }

    //returns the probability numerator.
    //dock counting starts at 0.
    public static BigInteger getProbabilityNumerator(int d) {
        return probNumerator.get(d);
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
            probNumerator.clear();
            for (int i = 0; i < f; i++) {
                probNumerator.add(BigInteger.ONE);
            }

            Integer den = f;
            probDenominator = probDenominator.multiply(new BigInteger(den.toString()));
        }
        //non-trivial case: more than 1 die
        else if (d > 1) {
            //use recursion
            setupProbabilities(d - 1, f);

            //copy over previous values
            ArrayList<BigInteger> probCopy = new ArrayList<BigInteger>();
            for (int i = 0; i < probNumerator.size(); i++) {
                probCopy.add(probNumerator.get(i));
            }

            //clear probabilities
            probNumerator.clear();
            //add 0 to first index
            probNumerator.add(BigInteger.ZERO);

            //add the values of probcopy
            for (int i = 1; i <= f; i++) { //iterate from 1 to f
                for (int j = 0; j < (d - 1) * f; j++) {
                    if (i + j == probNumerator.size()) {
                        //add new element
                        probNumerator.add(probCopy.get(j));
                    }
                    else {
                        //add to existing element
                        probNumerator.set(i + j, probNumerator.get(i + j).add(probCopy.get(j)));
                    }
                }
            }
            
            //update denominator
            Integer subden = f;
            BigInteger subDenominator = new BigInteger(subden.toString());
            probDenominator = probDenominator.multiply(subDenominator);
        }
    }

    //sets the value of usable, i.e. if the dock number can be rolled.
    public void setUsable(boolean useValue) {
        usable = useValue;
    }
}