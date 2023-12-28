import java.util.ArrayList;
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

    //returns the expected duration of the position.
    //ed is in the form of an array ed={Numerator, Denominator}.
    public BigInteger[] expectedDurationRecursive()  {
        BigInteger ed[] = {BigInteger.ONE, BigInteger.ONE}; //output variable
        BigInteger sumProb = BigInteger.ZERO; //sum of probabilities of docks with chips in them.
        BigInteger tempInt = BigInteger.ZERO; //used as a placeholder for the LCD and GCF when they are computed.

        //trivial cases
        //chip in non-usable dock
        for (int i = 0; i < docks.size(); i++) {
            if (!docks.get(i).isUsable() && getChips(i) > 0) {
                ed[0] = BigInteger.ONE;
                ed[1] = BigInteger.ZERO;
                return ed;
            }
        }
        //no chips
        if (count == 0) {
            ed[0] = BigInteger.ZERO;
            ed[1] = BigInteger.ONE;
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
                        BigInteger[] ed_sub = p.expectedDurationRecursive();

                        //ed_sub * prob[i]
                        ed_sub[0] = ed_sub[0].multiply(Dock.getProbabilityNumerator(i));
                        ed_sub[1] = ed_sub[1].multiply(Dock.getProbabilityDenominator());

                        //add to ed. use lcm
                        //get greatest common divisor
                        tempInt = ed[1].gcd(ed_sub[1]);
                        //get new denominator. lcm = denominator1 * denominator2 / gcd
                        ed[1] = ed[1].multiply(ed_sub[1]);
                        ed[1] = ed[1].divide(tempInt);

                        //get new numerator.
                        //new numerator = lcm / old denominator * old numerator
                        //then new numerator1 = (den1 * den2 * num1) / (den1 * gcd)
                        //new numerator1 = den2 * num1 / gcd
                        //multiply ed[0] by ed_sub[1] then divide by tempInt.
                        ed[0] = ed[0].multiply(ed_sub[1]).divide(tempInt);
                        //new numerator2 = num2 * lcm / den2
                        //multiply ed_sub[0] by ed[1] then divide by tempDec.
                        ed_sub[0] = ed_sub[0].multiply(ed[1]).divide(ed_sub[1]);

                        //add the new numerators.
                        ed[0] = ed[0].add(ed_sub[0]);
                    }
                }
            }

            //divide ed by sumProb
            ed[0] = ed[0].multiply(Dock.getProbabilityDenominator());
            ed[1] = ed[1].multiply(sumProb);

            //put it in lowest terms.
            tempInt = ed[0].gcd(ed[1]); //get greatest common divisor
            ed[0] = ed[0].divide(tempInt); //divide both numerator and denominator by gcd
            ed[1] = ed[1].divide(tempInt);
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
    public BigInteger[][] gFunctionRecursive(Position p) {
        BigInteger[][] gFunction = {{BigInteger.ZERO, BigInteger.ONE}, {BigInteger.ZERO, BigInteger.ONE}, {BigInteger.ZERO, BigInteger.ONE}};
        int count1 = getCount();
        int count2 = p.getCount();
        BigInteger sumProb = BigInteger.ZERO;
        BigInteger tempInt[] = {BigInteger.ZERO, BigInteger.ZERO};

        //trivial cases
        if (count1 == 0 && count2 > 0) {
            //this wins
            gFunction[0][0] = BigInteger.ONE;
        }
        else if (count1 > 0 && count2 == 0) {
            //p wins
            gFunction[1][0] = BigInteger.ONE;
        }
        else if (count1 == 0 && count2 == 0) {
            //tie game
            gFunction[2][0] = BigInteger.ONE;
        }
        else if (isSamePosition(p)) {
            //identical positions guarantee a tie
            gFunction[2][0] = BigInteger.ONE;
        }
        else {
            //non-trivial case: game has not ended, and no outcome is guaranteed
            //copy variables
            BigInteger[][] g_sub;
            Position pos1, pos2;

            //loop through each dock
            //get sumProb
            for (int i = 0; i < currentDocks; i++) {
                if (getChips(i) > 0 || p.getChips(i) > 0) {
                    //add prob[i] to sumProb
                    sumProb = sumProb.add(Dock.getProbabilityNumerator(i));
                }
            }

            for (int i = 0; i < currentDocks; i++) {
                if (getChips(i) > 0 || p.getChips(i) > 0) {
                    //copy this and p into pos1 and pos2
                    pos1 = new Position(this);
                    pos2 = new Position(p);

                    //remove a chip from dock i
                    pos1.removeChip(i, 1);
                    pos2.removeChip(i, 1);

                    //call gFunctionRecursive on copied positions
                    g_sub = pos1.gFunctionRecursive(pos2);

                    //multiply g_sub by prob[i]/sumProb
                    g_sub[0][0] = g_sub[0][0].multiply(Dock.getProbabilityNumerator(i));
                    g_sub[0][1] = g_sub[0][1].multiply(sumProb);

                    g_sub[1][0] = g_sub[1][0].multiply(Dock.getProbabilityNumerator(i));
                    g_sub[1][1] = g_sub[1][1].multiply(sumProb);

                    g_sub[2][0] = g_sub[2][0].multiply(Dock.getProbabilityNumerator(i));
                    g_sub[2][1] = g_sub[2][1].multiply(sumProb);

                    //add to current value of gFunction. use LCM to add and GCD to put into lowest terms.
                    //gFunction[0] denominator
                    tempInt[0] = gFunction[0][1].gcd(g_sub[0][1]); //gcd as integer
                    tempInt[1] = gFunction[0][1]; //hold the value of denominator
                    gFunction[0][1] = gFunction[0][1].multiply(g_sub[0][1]).divide(tempInt[0]); //lcm = denominator1 * denominator2 / gcd

                    //gFunction[0] numerator
                    gFunction[0][0] = gFunction[0][0].multiply(gFunction[0][1]).divide(tempInt[1]); //numerator = lcm / denominator1 * numerator1
                    g_sub[0][0] = g_sub[0][0].multiply(gFunction[0][1]).divide(g_sub[0][1]); //numerator = lcm / denominator2 * numerator2
                    gFunction[0][0] = gFunction[0][0].add(g_sub[0][0]); //add the two numerators

                    //gFunction[1] denominator
                    tempInt[0] = gFunction[1][1].gcd(g_sub[1][1]); //gcd as integer
                    tempInt[1] = gFunction[1][1]; //hold the value of denominator
                    gFunction[1][1] = gFunction[1][1].multiply(g_sub[1][1]).divide(tempInt[0]); //lcm = denominator1 * denominator2 / gcd

                    //gFunction[1] numerator
                    gFunction[1][0] = gFunction[1][0].multiply(gFunction[1][1]).divide(tempInt[1]); //numerator = lcm / denominator1 * numerator1
                    g_sub[1][0] = g_sub[1][0].multiply(gFunction[1][1]).divide(g_sub[1][1]); //numerator = lcm / denominator2 * numerator2
                    gFunction[1][0] = gFunction[1][0].add(g_sub[1][0]); //add the two numerators

                    //gFunction[2] denominator
                    tempInt[0] = gFunction[2][1].gcd(g_sub[2][1]); //gcd as integer
                    tempInt[1] = gFunction[2][1]; //hold the value of denominator
                    gFunction[2][1] = gFunction[2][1].multiply(g_sub[2][1]).divide(tempInt[0]); //lcm = denominator1 * denominator2 / gcd

                    //gFunction[2] numerator
                    gFunction[2][0] = gFunction[2][0].multiply(gFunction[2][1]).divide(tempInt[1]); //numerator = lcm / denominator1 * numerator1
                    g_sub[2][0] = g_sub[2][0].multiply(gFunction[2][1]).divide(g_sub[2][1]); //numerator = lcm / denominator2 * numerator2
                    gFunction[2][0] = gFunction[2][0].add(g_sub[2][0]); //add the two numerators
                }
            }

            //gFunction[0]
            //put into lowest terms
            tempInt[0] = gFunction[0][0].gcd(gFunction[0][1]); //get gcd
            gFunction[0][0] = gFunction[0][0].divide(tempInt[0]); //divide by gcd
            gFunction[0][1] = gFunction[0][1].divide(tempInt[0]); //divide by gcd

            //gFunction[1]
            //put into lowest terms
            tempInt[0] = gFunction[1][0].gcd(gFunction[1][1]); //get gcd
            gFunction[1][0] = gFunction[1][0].divide(tempInt[0]); //divide by gcd
            gFunction[1][1] = gFunction[1][1].divide(tempInt[0]); //divide by gcd

            //gFunction[2]
            //put into lowest terms
            tempInt[0] = gFunction[2][0].gcd(gFunction[2][1]); //get gcd
            gFunction[2][0] = gFunction[2][0].divide(tempInt[0]); //divide by gcd
            gFunction[2][1] = gFunction[2][1].divide(tempInt[0]); //divide by gcd
        }

        return gFunction;
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