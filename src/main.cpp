#include <iostream>
#include <fstream>

enum gamestate {ongoing, draw, black, white};

char** createMatrix(int, int);
void destroyMatrix(char**, int);
void initBoard(char**, int, std::ifstream&);
void displayBoard(char**, int);
bool validSrc(char**, int, int, int, int);

int main() {
	const int size = 8;
	char** board = createMatrix(size, size);
	int turn = 0; // Starts from white (even numbers are white)
	gamestate curState = ongoing;

	std::ifstream initFile("init.txt");
	initBoard(board, size, initFile);
	initFile.close();
	displayBoard(board, size);

	while(curState == ongoing) {

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
	if (board[row][col] == '.') return false;
	if (turn % 2 != 0 && board[row][col] >= 'A' && board[row][col] <= 'Z') return true;
	if (turn % 2 == 0 && board[row][col] >= 'a' && board[row][col] <= 'z') return true;
	return false;
}
