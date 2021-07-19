#include <iostream>
#include <stdlib.h>
#include <time.h> 
#include <string>
#include <sstream>
using namespace std;

#define MAXSIDE  25 // maximum grid size

struct Mine { //struct for representing the mines' coordinates (needed for power up)
	int row, col;

	Mine(int r, int c) {
		row = r;
		col = c;
	};

	Mine() {};
};

int numMines, side; //number of mines and size of grid defined by player

Mine* mines; //array for keeping the mines' coordinates

char** gameBoard; //gameBoard keeps the information of the whole grid, playerBoard is what is shown to the player
char** playerBoard;

int revealedCells = 0; //number of cells that have been revealed to the player - necessary for detecting win condition

bool gameOver = false; //boolean variable that triggers the Game Over prompt

bool hasMetalDetector = true; //boolean to check if the player has already used power-up



void printBoard(char** board ) //print any board
{
	for (int i = 0; i < side; i++) {
		printf("%2d ", i);
		for (int j = 0; j < side; j++) {
			printf("%2c ", board[i][j]);
		}
		printf("\n");
	}

	printf("%2c ", ' ');
	
	for (int i = 0; i < side; i++) {
		printf("%2d ", i);
	}

	printf("\n");
}

void setGameOptions() { //prompt player to set game options (grid size and number of mines)
	printf("Welcome to Minesweeper! Please enter the size of the side of the game board (Max: 25):\n");
	string input = "";
	while (true) {
		getline(cin, input);
		stringstream myStream(input);
		if (myStream >> side) {
			if (side <= MAXSIDE) break;
		}						
		printf("Invalid number, please try again (maximum side size is 25).\n");
	}
	printf("Please enter the amount of mines to be placed on the field (Max: %d):\n", (side*side)-1); //max amount of mines is set to be so that at least 1 space of the board will be empty
	while (true) {
		getline(cin, input);
		stringstream myStream(input);
		if (myStream >> numMines) {
			if (numMines <= (side*side) - 1) break;
		}
		printf("Invalid number, please try again (maximum number of mines is %d).\n", (side*side) - 1);
	}
}

void initialize() //intialize both boards
{
	gameBoard = new char*[side];
	playerBoard = new char*[side];
	//fill the grid with empty spaces
	for (int i = 0; i < side; i++) {
		gameBoard[i] = new char[side];
		playerBoard[i] = new char[side];
		for (int j = 0; j < side; j++) {		
			gameBoard[i][j] = playerBoard[i][j] = '-';
		}
	}

	//spread the mines by generating random positions
	mines = new Mine[numMines];
	srand(time(NULL));
	for (int i = 0; i < numMines;) {
		int row = rand() % side;
		int col = rand() % side;
		if (gameBoard[row][col] != '*') {
			gameBoard[row][col] = '*';
			mines[i] = Mine(row, col); //store the mine coordinate
			i++;
		}		
	}
}

bool isValidPos(int row, int col) // check if a given position is within the board
{
	return (row >= 0) && (row < side) && (col >= 0) && (col < side);
}

void revealPos(int row, int col) { //recursive function for revealing the playerBoard cells
	if (playerBoard[row][col] != '-') return; //stop condition - if this cell has already been revealed, stop recursion
	int mineCount = 0; //variable for counting the number of adjacent mines
	if (gameBoard[row][col] == '*') { //check if the player chose to reveal a mine - trigger game over
		playerBoard[row][col] = '*';
		gameOver = true;
		return;
	}
	else { //if the chosen tile is not a mine, count how many adjacent cells have mines
		if (isValidPos(row-1, col-1) == true) {
			if (gameBoard[row-1][col-1] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row-1, col) == true) {
			if (gameBoard[row-1][col] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row-1, col+1) == true) {
			if (gameBoard[row-1][col+1] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row, col-1) == true) {
			if (gameBoard[row][col-1] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row, col+1) == true) {
			if (gameBoard[row][col+1] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row+1, col-1) == true) {
			if (gameBoard[row+1][col-1] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row+1, col) == true) {
			if (gameBoard[row+1][col] == '*') {
				mineCount++;
			}
		}
		if (isValidPos(row+1, col+1) == true) {
			if (gameBoard[row+1][col+1] == '*') {
				mineCount++;
			}
		}
	}
	if (mineCount > 0) { //after counting, if there are ajdacent mines near the cell, reveal it with the number of mines nearby
		playerBoard[row][col] = mineCount + '0';
		revealedCells++;
	}
	else { //if the cell has no adjacent mines, recursively reveal it's neighbours
		playerBoard[row][col] = 'x';
		revealedCells++;
		if (isValidPos(row-1, col-1) == true) {
			revealPos(row-1, col-1);
		}
		if (isValidPos(row-1, col) == true) {
			revealPos(row-1, col);
		}
		if (isValidPos(row-1, col+1) == true) {
			revealPos(row-1, col+1);
		}
		if (isValidPos(row, col-1) == true) {
			revealPos(row, col-1);
		}
		if (isValidPos(row, col+1) == true) {
			revealPos(row, col+1);
		}
		if (isValidPos(row+1, col-1) == true) {
			revealPos(row+1, col-1);
		}
		if (isValidPos(row+1, col) == true) {
			revealPos(row+1, col);
		}
		if (isValidPos(row+1, col+1) == true) {
			revealPos(row+1, col+1);
		}		
	}
}

void revealBomb() { //reveals a random bomb tile when player uses the power-up
	srand(time(NULL));
	int mineIndex = rand() % numMines;
	Mine revealedMine = mines[mineIndex];
	playerBoard[revealedMine.row][revealedMine.col] = '*';

}

void gameLoop() { //main game loop function
	setGameOptions();
	initialize();
	//printBoard(gameBoard); - print the actual game board before the game for debug purposes
	printBoard(playerBoard);
	string input = "";
	int row, col, c;
	while (revealedCells < (side * side) - numMines && gameOver == false) { //main game loop - stop only if the player has revealed all cells that are not bombs or if there was a game over
		//obtain player input
		printf("Choose a cell to reveal (row, column) or enter \"F\" to use the metal detector:\n");
		getline(cin, input);
		if (input.length() == 1) {
			if (input[0] == 'F' || input[0] == 'f') { //case the player wants to use the power-up
				if (hasMetalDetector == true) {
					revealBomb();
					printBoard(playerBoard);
					hasMetalDetector = false;
					continue;
				}
				else {
					printf("You already used your metal detector!\n");
					continue;
				}
				
			}			
		}
		else {
			if (input.find_first_of("0123456789") != std::string::npos) { //read coordinates of player input
				size_t pos;
				if (pos = input.find(" ") != std::string::npos) {
					row = stoi(input.substr(0, pos + 1));
					col = stoi(input.substr(pos + 1));
					if (isValidPos(row, col) == false) {
						printf("You must enter a valid coordinate. Try again. (Ex: \"2 3\")\n");
						continue;
					}
				}
				else {
					printf("You must enter a valid coordinate. Try again. (Ex: \"2 3\")\n");
					continue;
				}
				
			}else{
				printf("Please enter a valid coordinate (Ex. \"2 3\") or enter \"F\" to use the metal detector.\n");
				continue;
			}
		}
		//reveal the requested position, update board and how many unrevealed cells are left
		revealPos(row, col); 
		printBoard(playerBoard);
		printf("Cells left: %d\n", (side * side) - revealedCells);
	}
	if (gameOver == true) { //game over scenario
		printf("Game over! Would you like to play again? (y/n) ");
		char answer;
		scanf_s(" %c", &answer, 2);
		if (answer == 'y') {
			gameOver = false;
			revealedCells = 0;
			delete(gameBoard);
			delete(playerBoard);
			delete(mines);
			gameLoop();
		}
		else exit(0);
	}
	if (revealedCells == (side * side) - numMines) { //win scenario
		printf("Congratulations! You won!\n");
		printf("Would you like to play again ? (y / n) ");
		char answer;
		scanf_s(" %c", &answer, 2);
		if (answer == 'y') {
			gameOver = false;
			revealedCells = 0;
			gameLoop();
		}
		else exit(0);
	}
}

int main()
{
	gameLoop();	
}
