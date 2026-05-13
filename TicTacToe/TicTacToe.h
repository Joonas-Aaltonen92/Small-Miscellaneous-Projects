#pragma once
#include <array>
#include <iostream>
class TicTacToe
{
	std::array<char, 9> game{};
	bool currentPlayer = false;

	static constexpr std::array<std::array<std::size_t, 3>, 8> winningConditions = { {
		{0,1,2},{3,4,5},{6,7,8},//Lines
		{0,3,6},{1,4,7},{2,5,8},//Columns
		{0,4,8},{2,4,6}			//Diagonals
} };
public:
	TicTacToe() { game.fill(' '); }
	void play();
	void insert(std::array<char, 9>& array);

	void changePlayer() { currentPlayer = !currentPlayer; }
	void printGame(std::array<char,9>& array);
	char getPlayer(){return currentPlayer ? 'O' : 'X';}
	void clearScreen() { std::system("cls"); }
	bool checkWin(std::array<char, 9>& array);
	bool checkDraw(std::array<char, 9>& array);
};

