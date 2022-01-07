# LUDO_Game

--PSEUDOCODE AND OS CONCEPTS EXPLANATION--

void* KILLER(void* arg) {
This function is used by KILLER_THREAD and is responsible for keeping track and 
checking of all the grid coordinates. If the coordinates of current token matches any of the other 
already present token coordinates, it kills that token and sends it back to home by decrements its 
index to 0 and decreases its score to 0 too. This function is called by thread after each players 
turn to keep track of the current grid situation.
}
Void* TOKEN_MOVEMENT () {
This function is used by PLAYER_THREAD and is responsible for token movements.
The semaphore that is incremented in EVENT_HAPPENING() is decremented here so no other 
thread uses the critical portion unless current one does it’s job. The argument here is converted to 
int type and that value is used to check which token should be moved and how much. This also 
keeps check that a player can’t start his game until first six comes.
}
Void player_thread_maker () {
This function is responsible for creating 4 player threads. This uses 
TOKEN_MOVEMENT function and sends the loops iterating value as argument which is used 
in TOKEN_MOVEMENT to determine color of tokens.
}
Int RANDOMIZER () {
This generates random values from 1 to 6. This value is used to roll the dice.
}
Class DICE_CLASS {
DICE_CLASS() {
Constructor of class that initializes x and y axis of dice.
}
Void SCORE() {
This function converts int value of score to string and displays it on the game 
window.
}
Void draw_DICE () {
Takes the value from RANDOMIZER() and draws an appropriate dice for 
whichever players turn it is.
}
Void draw_GOLLA () {
This function draws a circle of whichever token colors turn it is to let the players 
know.
}
};
Class Game {
Game () {
Constructor calls init_VARIABLES and init_WINDOW to create and initialize
everything.
}
Void init_VARIABLES() {
This function initializes all the variables used in class. This also sets the 
coordinated of board for every token in arrays.
}
Void init_WINDOW () {
This creates the game window.
}
Void init_BOARD () {
This creates the board of Ludo.
}
Bool OPENWINDOW () {
This simply returns that the window is still open so the game keeps on running.
}
Void GOTIAN () {
This function creates all 4 tokens by loading images from sfml sprites.
}
Void EVENT_HAPPENING () {
This function calls init_BOARD to draw game board and then draws dice too 
from DICE CLASS. Here the events are specified that when what event is done so the working is 
according to it. For example, if esc is pressed then game window is closed and game terminates.
The main working of this function is that it checks when left mouse key is pressed. When 
it is pressed dice is rolled for one player and then semaphore of player thread is incremented 
through sem_post() so now the thread gets access to critical portion of function.
After this killer thread is made to check for whole grids value and kills the token.
}
Void UPDATE () {
This is called in main game loop in main function which keeps on drawing the 
game again and again.
}
Void RENDER () {
All the game changes and new updated drawings are done through this.
}
};
Int main () {
Firstly, the value of semaphore of player and semaphore dice are initialized to zero. Then 
a while loop is called which keeps on working until the game window is closed. In this loops 
UPDATE and RENDER from Game class are called to keep the game running. After this the 
join of player threads is done so they finish accordingly. 
Background music through sfml is also called here.
}
