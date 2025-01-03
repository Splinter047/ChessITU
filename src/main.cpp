#include <fstream>
#include <iostream>
#include <string.h>
#include <SFML/Graphics.hpp>

const int WIN_SIZE = 800; // Window size in pixels
const int WIDTH = 8; // Amount of Rows, Cols
const float BLOCK_SIZE = WIN_SIZE / WIDTH; // Size of each block in pixels

const sf::Color LIGHT_BLOCK(240, 217, 181); // Color of light block
const sf::Color DARK_BLOCK(181, 136, 99); // Color of dark block

void initBoard(std::ifstream&, char[WIDTH][WIDTH]);
void drawBoard(sf::RenderWindow&);
void drawPieces(sf::RenderWindow&, char[WIDTH][WIDTH]);
int strCharray(char* dest, const char* src);
int strCharray(char* dest, const char* src, int start);

int main()
{
	// Creating the window without resizing and enabling VSync
	auto window = sf::RenderWindow(sf::VideoMode({WIN_SIZE, WIN_SIZE}), "Chess", sf::Style::Close);
	window.setVerticalSyncEnabled(true);

	// Create board
	char board[WIDTH][WIDTH]{'.'};

	// Initialize board from file
	std::ifstream initLayout("init.txt");
	initBoard(initLayout, board);
	initLayout.close();

	// Turn counter, even -> white | odd -> black
	int turn = 0;

	// Main loop
	while (window.isOpen())
	{
		// Handles the close button
		while (const std::optional event = window.pollEvent())
		{
			if (event->is<sf::Event::Closed>()) window.close();
		}

		window.clear();
		drawBoard(window);
		drawPieces(window, board);

		// Outputs anything drawn to the window
		window.display();
	}
}

void initBoard(std::ifstream &file, char mat[WIDTH][WIDTH]) {
	for (int row = 0; row < WIDTH; row++) {
		for (int col = 0; col < WIDTH; col++) file >> (mat[row][col]);
		file.ignore();
	}
}

void drawBoard(sf::RenderWindow &window) {
	for (int row = 0; row < WIDTH; row++) {
		for (int col = 0; col < WIDTH; col++) {
			bool isLightSquare = (row + col) % 2 == 0;
			sf::RectangleShape square(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
			square.setFillColor(isLightSquare ? LIGHT_BLOCK : DARK_BLOCK);

			square.setPosition({col * BLOCK_SIZE, row * BLOCK_SIZE});

			square.setOutlineThickness(-1.0f);
			square.setOutlineColor(sf::Color::Black);

			window.draw(square);
		}
	}
}

void drawPieces(sf::RenderWindow &window, char mat[WIDTH][WIDTH]) {
	sf::Texture pieceImg;
	sf::Sprite piece(pieceImg);
	char* fileStr = new char[20]{'\0'};
	int idx = 0;

	for (int row = 0; row < WIDTH; row++) {
		for (int col = 0; col < WIDTH; col++) {
			if (mat[row][col] == '.') continue;

			if (mat[row][col] >= 'A' && mat[row][col] <= 'Z')
				idx = strCharray(fileStr, "textures/blk_");
			else if (mat[row][col] >= 'a' && mat[row][col] <= 'z')
				idx = strCharray(fileStr, "textures/wht_");

			fileStr[idx] = mat[row][col];
			idx = strCharray(fileStr, ".png", idx + 1);

			pieceImg.loadFromFile(fileStr);
			piece.setTexture(pieceImg, true);

			piece.setPosition({col * BLOCK_SIZE + 18, row * BLOCK_SIZE + 18});
			window.draw(piece);
		}
	}

	delete[] fileStr;
}

int strCharray(char* dest, const char* src) {
	int idx = 0;
	while (src[idx] != '\0') {
		dest[idx] = src[idx];
		idx++;
	}
	dest[idx] = '\0';
	return idx;
}

int strCharray(char* dest, const char* src, int start) {
	int idx = 0;
	while (src[idx] != '\0') {
		dest[start + idx] = src[idx];
		idx++;
	}
	dest[start + idx] = '\0';
	return start + idx;
}