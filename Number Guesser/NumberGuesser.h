#pragma once
#include <iostream>
#include <ctime>
#include <limits>
#include <string>

class NumberGuesser
{
	int max = 100;
	int numberToBeGuessed = 0;

public:
	NumberGuesser(int max = 100)
	{
		std::srand(static_cast<unsigned>(time(nullptr)));
		numberToBeGuessed = generateRandomNumber(max);
	}
	int generateRandomNumber(int max) { return rand() % (max + 1); }
	void play()
	{
		std::cout << "Welcome to Number Guesser!\n";
		std::cout << "Guess a number between 0 and " << max << ": ";
		while (true)
		{
			int guess = readValidGuess();
			if (isCorrect(guess))
				break;
		}
		std::cout << "Thanks for playing!\n";
		std::cout << "Press Enter to exit...";
		std::cin.get();
	}
	
	int readValidGuess()
	{
		std::string line;
		while (true)
		{
			std::cout << "Guess a number between 0 and " << max << ": ";
			if (!std::getline(std::cin,line))
			{
				std::cout << "Invalid input. Please try again.\n";
				continue;
			}
			try
			{
				size_t pos;
				int guess = std::stoi(line, &pos);
				if (pos == line.size() && guess >= 0 && guess <= max)
					return guess;
				std::cout << "Please enter only a number(no extra characters).\n";
			}
			catch (...)
			{
				std::cout << "Invalid input. Please enter a valid number within range.\n";
			}
		}
	}

	bool isCorrect(int& number)
	{
		if (number == numberToBeGuessed)
		{
			std::cout << "Congratulations! You guessed the correct number: " << numberToBeGuessed << std::endl;
			return true;
		}
		
		if (number < numberToBeGuessed)
		{
			std::cout << "Too low! Guess again!\n";
			return false;
		}
		else
		{
			std::cout << "Too high! Guess again!\n";
			return false;
		}
	}
};

