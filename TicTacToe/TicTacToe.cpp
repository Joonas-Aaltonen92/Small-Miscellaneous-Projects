#include "TicTacToe.h"
#include <iomanip>
#include <string>

void TicTacToe::play()
{
	std::cout << "Let's play Tic Tac Toe!\n\n";
	while (true)
	{
		printGame(game);
		insert(game);
		if (checkWin(game))
		{
			//See which player won
			std::string playerName = (getPlayer() == 'O') ? "Player 1 (O)" : "Player 2 (X)";
			std::cout << "\n\n" << playerName <<" won the game!\n";
			break;
		}
		else if (checkDraw(game))
		{
			std::cout << "\nIt's a draw!\n";
			break;
		}
		changePlayer();
		clearScreen();
	}
}

void TicTacToe::insert(std::array<char, 9>& array)
{
	std::string line;
	while (true)
	{
		std::cout << "Select a position to insert your thing (0-8): ";
		if (!std::getline(std::cin, line))
		{
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}
		try
		{
			size_t pos;
			int position = std::stoi(line, &pos);
			if (pos == line.size() && position >= 0 && position <= 8) //input was int
			{
				if (array[position] != ' ')
					std::cout << "Occupied! Try again!\n";
				else
				{
					array[position] = getPlayer();
					clearScreen();
					printGame(array);
					break;
				}
			}
			std::cout << "Please enter only a number between 0-8 (no extra characters).\n";
		}
		catch (...)
		{
			std::cout << "Invalid input. Please enter a valid number within range.\n";
		}
	}
}

void TicTacToe::printGame(std::array<char, 9>& array)
{
	for (int i = 0; i < 3; ++i)
	{
		std::cout << "  |"; //Left padding

		for (int j = 0; j < 3; ++j)
		{
			std::cout << std::setw(4) << array[(i*3)+j];
			if (j < 2)
				std::cout << "|";
		}
		std::cout << "|\n";
		if (i < 2)
			std::cout << "  -------------\n";
	}
	std::cout << "\n";
}

bool TicTacToe::checkWin(std::array<char, 9>& array)
{
	for (const auto& line : winningConditions)
	{
		if (array[line[0]] == getPlayer() && array[line[1]] == getPlayer() && array[line[2]] == getPlayer())
			return true;
	}
	return false;
}

bool TicTacToe::checkDraw(std::array<char, 9>& array)
{
	for (int i = 0; i < 9; ++i)
	{
		if (array[i] == ' ')
			return false; //Empty places left, game isn't over yet.
	}
	//All places have been occupied, but nobody won -> draw
	return true;
}
