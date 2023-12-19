import java.util.ArrayList;
import java.math.BigDecimal;
import java.math.BigInteger;

public class Position {
    //character representations of numbers 1 through 35.
    static final String DOCK_STRING = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

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

    //constructor given number of dice.
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

    //constructor given a position to copy.
    public Position(Position p) {
        count = p.getCount();

        for (int i = 0; i < currentDocks; i++) {
            docks.add(new Dock(p.getChips(i), p.isDockUsable(i)));
        }
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

    //returns the expected duration of the position.
    //ed is in the form of an array ed={Numerator, Denominator}.
    public BigDecimal[] expectedDurationRecursive()  {
        BigDecimal ed[] = {BigDecimal.ONE, BigDecimal.ONE}; //output variable
        BigDecimal sumProb = BigDecimal.ZERO; //sum of probabilities of docks with chips in them.
        BigInteger edInt[] = {BigInteger.ONE, BigInteger.ONE}; //used to get LCD when adding fractions and GCF when dividing fractions.
        BigInteger tempInt = BigInteger.ZERO; //used as a placeholder for the LCD and GCF when they are computed.
        BigDecimal tempDec; //big decimal version of tempInt.

        //trivial cases
        //chip in non-usable dock
        for (int i = 0; i < docks.size(); i++) {
            if (!docks.get(i).isUsable() && getChips(i) > 0) {
                ed[0] = BigDecimal.ONE;
                ed[1] = BigDecimal.ZERO;
                return ed;
            }
        }
        //no chips
        if (count == 0) {
            ed[0] = BigDecimal.ZERO;
            ed[1] = BigDecimal.ONE;
        }
        //non-trivial case
        else {
            //sum of probabilities
            for (int i = 0; i < currentDocks; i++) {
                if (getChips(i) > 0) {
                    sumProb = sumProb.add(Dock.getProbabilityNumerator(i));
                    //use recursion to get ED of reduced position
                    if (count > 1) {
                        Position p = new Position(this);
                        p.removeChip(i, 1);
                        BigDecimal[] ed_sub = p.expectedDurationRecursive();

                        //ed_sub * prob[i]
                        ed_sub[0] = ed_sub[0].multiply(Dock.getProbabilityNumerator(i));
                        ed_sub[1] = ed_sub[1].multiply(Dock.getProbabilityDenominator());

                        //add to ed. use lcd
                        //get greatest common divisor
                        edInt[0] = ed[1].toBigInteger();
                        edInt[1] = ed_sub[1].toBigInteger();
                        tempInt = edInt[0].gcd(edInt[1]);
                        //get new denominator. lcm = denominator1 * denominator2 / gcd
                        tempDec = new BigDecimal(tempInt);
                        ed[1] = ed[1].multiply(ed_sub[1]);
                        ed[1] = ed[1].divide(tempDec);

                        //get new numerator.
                        //new numerator = lcm / old denominator * old numerator
                        //set tempdec to ed[1]
                        tempDec = new BigDecimal(edInt[0]);
                        //multiply ed[0] by ed[1] then divide by tempDec.
                        ed[0] = ed[0].multiply(ed[1]).divide(tempDec);
                        //set tempdec to ed_sub[1]
                        tempDec = new BigDecimal(edInt[1]);
                        //multiply ed_sub[0] by ed[1] then divide by tempDec.
                        ed_sub[0] = ed_sub[0].multiply(ed[1]).divide(tempDec);

                        //add the new numerators.
                        ed[0] = ed[0].add(ed_sub[0]);
                    }
                }
            }

            //divide ed by sumProb
            ed[0] = ed[0].multiply(Dock.getProbabilityDenominator());
            ed[1] = ed[1].multiply(sumProb);

            //put it in lowest terms.
            edInt[0] = ed[0].toBigInteger(); //convert to big integer
            edInt[1] = ed[1].toBigInteger();
            tempInt = edInt[0].gcd(edInt[1]); //get greatest common divisor
            tempDec = new BigDecimal(tempInt); //convert back to big decimal
            ed[0] = ed[0].divide(tempDec); //divide both numerator and denominator by gcd
            ed[1] = ed[1].divide(tempDec);
        }

        return ed;
    }

    //returns the expected duration of the position.
    //ed is in the form of an array ed={numerator, denominator}.
    //for now, it is unlikely that this will reach the double format limit.
    public double[] expectedDurationSimulation(double interval) {
        double ed[] = {0d, 0d};
        double rolls = 0d;
        double games = 0d;
        Position p;
        int roll;

        //trivial case: chip in impossible dock
        for (int i = 0; i < docks.size(); i++) {
            if (!docks.get(i).isUsable() && getChips(i) > 0) {
                ed[0] = 1d;
                ed[1] = 0d;
                return ed;
            }
        }

        //non-trivial case: normal position
        //assume that Game.setDice has already been called
        do {
            p = new Position(this);

            while (p.getCount() > 0) {
                roll = Game.rollDice();
                p.removeChip(roll - 1, 1);
                rolls++;
            }
            games++;
        } while (games < interval);

        //numerator is always rolls
        ed[0] = rolls;        
        //denominator is always interval
        ed[1] = interval;

        return ed;
    }

    //returns the number of chips in dock d.
    //dock counting starts at 1.
    public int getChips(int d) {
        return docks.get(d).chipCount();
    }

    //returns the number of chips in the position.
    public int getCount() {
        return count;
    }

    //returns the number of docks.
    public static int getDocks() {
        return currentDocks;
    }

    //returns the g Function values for the position and its opponent.
    public BigDecimal[][] gFunctionRecursive(Position p) {
        BigDecimal[][] gFunction = {{BigDecimal.ZERO, BigDecimal.ONE}, {BigDecimal.ZERO, BigDecimal.ONE}, {BigDecimal.ZERO, BigDecimal.ONE}};
        int count1 = getCount();
        int count2 = p.getCount();
        BigDecimal sumProb = BigDecimal.ZERO;
        BigInteger gInt[] = {BigInteger.ZERO, BigInteger.ZERO};
        BigInteger tempInt = BigInteger.ZERO;
        BigDecimal tempDec = BigDecimal.ZERO;

        //trivial cases
        if (count1 == 0 && count2 > 0) {
            //this wins
            gFunction[0][0] = BigDecimal.ONE;
        }
        else if (count1 > 0 && count2 == 0) {
            //p wins
            gFunction[1][0] = BigDecimal.ONE;
        }
        else if (count1 == 0 && count2 == 0) {
            //tie game
            gFunction[2][0] = BigDecimal.ONE;
        }
        else if (isSamePosition(p)) {
            //identical positions guarantee a tie
            gFunction[2][0] = BigDecimal.ONE;
        }
        else {
            //non-trivial case: game has not ended, and no outcome is guaranteed
            //copy variables
            BigDecimal[][] g_sub;
            Position pos1, pos2;

            //loop through each dock
            for (int i = 0; i < currentDocks; i++) {
                if (getChips(i) > 0 || p.getChips(i) > 0) {
                    //add to sumProb
                    sumProb = sumProb.add(Dock.getProbabilityNumerator(i));

                    //copy this and p into pos1 and pos2
                    pos1 = new Position(this);
                    pos2 = new Position(p);

                    //remove a chip from dock i
                    pos1.removeChip(i, 1);
                    pos2.removeChip(i, 2);

                    //call gFunctionRecursive on copied positions
                    g_sub = pos1.gFunctionRecursive(pos2);

                    //add to current value of gFunction. use LCM to add and GCD to put into lowest terms.
                    //gFunction[0] denominator
                    gInt[0] = gFunction[0][1].toBigInteger(); //denominator 1
                    gInt[1] = g_sub[0][1].toBigInteger(); //denominator 2
                    tempInt = gInt[0].gcd(gInt[1]); //gcd as integer
                    tempDec = new BigDecimal(tempInt); //gcd as float
                    gFunction[0][1] = gFunction[0][1].multiply(g_sub[0][1]).divide(tempDec); //lcm = denominator1 * denominator2 / gcd

                    //gFunction[0] numerator
                }
            }

            //divide by sumProb
        }

        return gFunction;
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

    //returns whether dock d is usable.
    //dock counting starts at 0.
    public boolean isDockUsable(int d) {
        return docks.get(d).isUsable();
    }

    //returns whether this is identical to p.
    public boolean isSamePosition(Position p) {
        boolean isSame = true;

        for (int i = 0; i < currentDocks; i++) {
            if (getChips(i) != p.getChips(i)) {
                isSame = false;
                break;
            }
        }

        return isSame;
    }

    //finds the dock with the most chips and returns the number of chips.
    public int maxDockChips() {
        int max = 0;

        for (Dock d : docks) {
            if (d.chipCount() > max) {
                max = d.chipCount();
            }
        }

        return max;
    }

    //removes num number of chips from dock d.
    //dock counting starts at 0.
    public int removeChip(int d, int num) {
        int removed;
        if (d < docks.size()) {
            removed = docks.get(d).removeChip(num);
            count -= removed;
        }
        else {
            throw new ArrayIndexOutOfBoundsException("No dock with that number in docks");
        }
        return removed;
    }

    //sets the value of currentDocks and usableDocks.
    public static void setDocks(int docks, int dice) {
        currentDocks = docks;
        usableDocks = currentDocks - dice + 1;
    }
}