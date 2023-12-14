# river-crossing-math
Computes probabilities for the River Crossing Game based on the article ["The River Crossing Game" (2007)](https://www.jstor.org/stable/27642986) by David Goering and Dan Canada.

rivercrossinggame.c is made with some Linux-specific commands. A Windows-compatible version is available with the same functionality.
RiverCrossingGame.java is being built to have the same functionality.

The programs will have three primary purposes:
1. Play an instance of the River Crossing Game: a game of probability. There are twelve docks on a river, numbered 1 through 12. Two players put chips on the docks as they wish. Two six-sided dice are then rolled. The numbers are added, and a chip is taken from the dock corresponding to the sum. If there are multiple chips in a dock, only one is taken. A player wins if all their chips are taken, i.e. everyone has "crossed the river".
1. Compute the expected duration of an initial position: how many rolls are expected to clear one side of the board.  
  i. Find the initial position with the lowest expected duration.
1. Compute the probability of game outcomes given an initial position on either side of the river.  
  i. Find the optimal initial position: the position with an equal or greater probability to win against all other initial positions.

---

Current Tasks: 
1. Finish the Java version of the program.
2. Use Python to investigate an "optimal shape" for optimal initial positions no matter how many chips.

---

The 6x2 notation stands for two six-sided dice.  
[Main Google Drive Folder](https://drive.google.com/drive/folders/1I5TYPwEBFRrDfN6z1BiaCVeDfi3OA7x-?usp=sharing)  
[6x2 Execution Times for Game Outcomes](https://docs.google.com/spreadsheets/d/1uC53uswlDVz2WOc44bhk9_Klw14liI4sK6TcbOOoY4Y/edit?usp=sharing)  
[6x2 Dataset of All Initial Position Game Outcomes](https://drive.google.com/drive/folders/13NBHfZ7kD6RT5n6ISt9VgHvYTSlCX460?usp=sharing)  
