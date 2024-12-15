#include <iostream>
#include <fstream>

enum gamestate {ongoing, draw, black, white};

// For phase 1 I am keeping things super simple, I am aware of plenty bugs, oversights
// and edge cases that I didnt handle, for the final project I plan to implement a gui anyways
// so there is a lot of restructuring that I will be doing. For now the basics are there :)

char** createMatrix(int, int);
void destroyMatrix(char**, int);
void initBoard(char**, int, std::ifstream&);
void displayBoard(char**, int);
bool validSrc(char**, int, int, int, int);
bool validDest(char**, int, int, int, int, int, int);
bool isPathClear(char**, int, int, int, int, int, int);
void makeMove(char**, int, int, int, int);

int main() {
	const int size = 8;
	char** board = createMatrix(size, size);
	int turn = 0; // Starts from white (even numbers are white)
	gamestate curState = ongoing;

	std::ifstream initFile("init.txt");
	initBoard(board, size, initFile);
	initFile.close();
	displayBoard(board, size);

	while (curState == ongoing) {
		int curRow, curCol, destRow, destCol;
		do {
			std::cout << "Select piece to move (row col): ";
			std::cin >> curRow >> curCol;
		} while (validSrc(board, size, curRow, curCol, turn));

		do {
			std::cout << "Select space to move to (row col): ";
			std::cin >> destRow >> destCol;
		} while (validDest(board, size, curRow, curCol, destRow, destCol, turn));

		makeMove(board, curRow, curCol, destRow, destCol);
		displayBoard(board, size);

		turn++;
		if(turn >= 20) curState = draw;
	}

	destroyMatrix(board, size);
}

char** createMatrix(int rows, int cols) {
	char** matrix = new char*[rows];

	for (int i = 0; i < rows; i++) {
		*(matrix + i) = new char[cols];
		for (int j = 0; j < cols; j++) *((*(matrix + i)) + j) = '\0';
	}
	return matrix;
}

void destroyMatrix(char** mat, int rows) {
	for (int i = 0; i < rows; i++) delete[] *(mat + i);
	delete[] mat;
}

void initBoard(char** board, int size, std::ifstream &file) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) board[i][j] = file.get();
		file.ignore();
	}
}

void displayBoard(char** board, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) std::cout << "|" << board[i][j];
		std::cout << "|\n";
	}
}

bool validSrc(char** board, int size, int row, int col, int turn) {
	if (row < 0 || row >= size || col < 0 || col >= size) return false;
	if (board[row][col] == '.') return false;
	if (turn % 2 != 0 && board[row][col] >= 'A' && board[row][col] <= 'Z') return true;
	if (turn % 2 == 0 && board[row][col] >= 'a' && board[row][col] <= 'z') return true;
	return false;
}

bool validDest(char** board, int size, int srcRow, int srcCol, int destRow, int destCol, int turn) {
	if (destRow < 0 || destRow >= size || destCol < 0 || destCol >= size) return false;
	const char piece = board[srcRow][srcCol];
	if (piece != '.') return false;

	switch (tolower(piece)) {
		case 'r': { // Rook
			if (srcRow == destRow) {
				int dirCol = (destCol > srcCol) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, 0, dirCol);
			} else if (srcCol == destCol) {
				int dirRow = (destRow > srcRow) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, dirRow, 0);
			}
			return false;
		}
		case 'n': { // Knight
			int dirRow = abs(destRow - srcRow);
			int dirCol = abs(destCol - srcCol);
			return (dirRow == 2 && dirCol == 1) || (dirRow == 1 && dirCol == 2);
		}
		case 'b': { // Bishop
			if (abs(destRow - srcRow) == abs(destCol - srcCol)) {
				int dirRow = (destRow > srcRow) ? 1 : -1;
				int dirCol = (destCol > srcCol) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, dirRow, dirCol);
			}
			return false;
		}
		case 'q': { // Queen
			if (srcRow == destRow) {
				int dirCol = (destCol > srcCol) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, 0, dirCol);
			} else if (srcCol == destCol) {
				int dirRow = (destRow > srcRow) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, dirRow, 0);
			} else if (abs(destRow - srcRow) == abs(destCol - srcCol)) {
				int dirRow = (destRow > srcRow) ? 1 : -1;
				int dirCol = (destCol > srcCol) ? 1 : -1;
				return isPathClear(board, srcRow, srcCol, destRow, destCol, dirRow, dirCol);
			}
			return false;
		}
		case 'k': { // King
			int dirRow = abs(destRow - srcRow);
			int dirCol = abs(destCol - srcCol);
			return dirRow <= 1 && dirCol <= 1;
		}
		case 'p': { // Pawn
			int direction = (isupper(piece)) ? -1 : 1; // Black moves up (-1), white moves down (+1)
			if (destCol == srcCol && board[destRow][destCol] == '.') {
				// Single move forward
				if (destRow - srcRow == direction) return true;

				// Double move forward (only from initial position)
				if ((srcRow == 1 && direction == 1) || (srcRow == 6 && direction == -1)) {
					if (destRow - srcRow == 2 * direction && board[srcRow + direction][srcCol] == '.') {
						return true;
					}
				}
			}
			return false;
		}
	}
	return false;
}

bool isPathClear(char** board, int srcRow, int srcCol, int destRow, int destCol, int dirRow, int dirCol) {
	int row = srcRow + dirRow, col = srcCol + dirCol;
	while (row != destRow || col != destCol) {
		if (board[row][col] != '.') return false;
		row += dirRow;
		col += dirCol;
	}
	return true;
}

void makeMove(char** board, int srcRow, int srcCol, int destRow, int destCol) {
	board[destRow][destCol] = board[srcRow][srcCol];
	board[srcRow][srcCol] = '.';
}