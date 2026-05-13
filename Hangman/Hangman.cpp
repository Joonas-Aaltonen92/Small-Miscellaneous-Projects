#include "Hangman.h"
#include <fstream>
#include <sstream>

void Hangman::run() {
	std::cout << "TOPIC: " << topic << "\n";

	std::vector<char> letterArray(word.length(),' ');
	std::vector<char> letterSpotArray(word.length(), '-');
	std::vector<char> wrongGuesses;
	bool isWin = false;
	bool isLost = false;

	while (1)
	{
		clearScreen();
		std::cout << "TOPIC: " << topic << "\n\n";
		printGameState();
		std::cout << "\n\nWORD: ";
		for (int i = 0; i < word.length(); ++i)
			std::cout << std::setw(3) << letterArray[i];
		std::cout << "\n" << "      ";
		for (int i = 0; i < word.length(); ++i)
			std::cout << std::setw(3) << letterSpotArray[i];

		std::cout << "\n\nWRONG GUESSES: ";
		if (!wrongGuesses.empty()) {
			for (int i = 0; i < wrongGuesses.size(); ++i)
				std::cout << std::setw(3) << wrongGuesses[i];
		}

		isWin = won(word,letterArray);
		if (isWin){
			std::cout << "\n\n\n--------------VICTORY!-------------";
			break;
		}
		if (isLost) {
			std::cout << "\n\n\n--------------YOU LOST!-------------";
			break;
		}

		std::cout << "\n\n";
		char guess = readValidGuess();
		std::vector<int> indexes;
		if (matchFound(word, guess, indexes)) {
			for (auto i : indexes)
				letterArray[i] = guess;
		}
		else {
			if (wrongGuesses.empty() || std::find(wrongGuesses.begin(), wrongGuesses.end(), guess) == wrongGuesses.end()) {
				wrongGuesses.push_back(guess);
				updateGameState(wrongGuessCount, isLost);
			}
		}
	}
}

char Hangman::readValidGuess() {
	std::string line;
	while (true) {
		std::cout << "Guess letter: ";
		if (!std::getline(std::cin, line)) {
			std::cout << "Invalid input. Please try again.\n";
			continue;
		}
		try {
			if (line.length() == 1 && std::isalpha(line[0])) {
				char c = std::toupper(line[0]);
				return c;
			}
			std::cout << "Please enter a valid letter of the alphabet.\n";
		}
		catch (...) {
			std::cout << "Invalid input, please enter a valid letter of the Roman alphabet.\n";
		}
	}
}

bool Hangman::matchFound(std::string word, char guess,std::vector<int>& indexes) {
	
	bool match = false;
	for (int i = 0; i < word.length(); ++i) {
		if (word[i] == guess) {
			indexes.push_back(i);
			match = true;
		}
	}
	return match;
}

bool Hangman::won(std::string word1, std::vector<char> word2) {
	for (int i = 0; i < word1.length(); ++i) {
		if (word[i] != word2[i])
			return false;
	}
	return true;
}

void Hangman::readFromFile(){
	if (!wordMap.empty())
		return;

	std::string filename("HANGMAN.TXT");
	std::fstream file(filename, std::ios::in | std::ios::out);
	if (!file.is_open()){
		std::cout << "Could not open file: " << filename << ", creating it out of thin air!\n";
		file.open(filename,std::ios::out);
	}

	std::string line;
	while (std::getline(file, line)){
		if (line.empty())
			continue;

		std::stringstream ss(line);
		std::string word;
		std::string topic;

		std::getline(ss,topic,';');
		std::getline(ss,word,';');

		if (!topic.empty() || !word.empty())
			wordMap[topic].push_back(word);
	}
}