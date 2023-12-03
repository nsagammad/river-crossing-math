# river-crossing-math
Computes probabilities for the River Crossing Game as seen in the article https://www.jstor.org/stable/27642986

rivercrossinggame.c is made with some Linux-specific commands. A Windows-compatible version is available with the same functionality.

The program will have three primary purposes:
1. Play an instance of the River Crossing Game: a game of probability. There are twelve docks on a river, numbered 1 through 12. Two players put chips on the docks as they wish. Two six-sided dice are then rolled. The numbers are added, and a chip is taken from the dock corresponding to the sum. If there are multiple chips in a dock, only one is taken. A player wins if all their chips are taken, i.e. everyone has "crossed the river".
2. Compute the expected duration of an initial position: how many rolls are expected to clear one side of the board.
3. Compute the probability of game outcomes given an initial position on either side of the river.

I am currently working on compiling the optimal initial position results in a Google Drive folder.
[Google Drive Folder](https://drive.google.com/drive/folders/1I5TYPwEBFRrDfN6z1BiaCVeDfi3OA7x-?usp=sharing)
[6x2 Execution Times](https://docs.google.com/spreadsheets/d/1uC53uswlDVz2WOc44bhk9_Klw14liI4sK6TcbOOoY4Y/edit?usp=sharing)
