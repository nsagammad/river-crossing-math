import java.util.ArrayList;

public class Position {
    private final String DOCK_STRING = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    private ArrayList<Dock> docks = new ArrayList<Dock>();
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
    }

    public int getCount() {
        return count;
    }

    public String docksToString() {
        String chipsStr = "";

        for (Dock d : docks) {
            if (d.chipCount() == 0) {
                chipsStr = chipsStr.concat("0");
            }
            else {
                chipsStr = chipsStr.concat(DOCK_STRING.substring(d.chipCount(), d.chipCount() + 1));
            }
        }
        
        return chipsStr;
    }
}