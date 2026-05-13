#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <random>
#include <iomanip>

class Hangman
{
	std::unordered_map<std::string, std::vector<std::string>> wordMap; //Key = topic, value = word to be guessed
	std::string word = "";
	std::string topic = "";
	int wrongGuessCount = 1;

	
	char hangman[14][9] = {
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ','T',' ',' ',
	' ','=','=','=','=','=','+','=','x',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ','/','|','\\',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' '
	};

	char blankState[14][9] = {
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ','T',' ',' ',
	' ','=','=','=','=','=','+','=','x',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ',' ','|',' ',' ',
	' ',' ',' ',' ',' ','/','|','\\',' ',
	' ',' ',' ',' ',' ',' ',' ',' ',' '
	};

	//Reference of a full hangman
	//char hangman[14][9] = {
	//' ',' ',' ',' ',' ',' ',' ',' ',' ',
	//' ',' ',' ',' ',' ',' ',' ',' ',' ',
	//' ',' ',' ',' ',' ',' ','T',' ',' ',
	//' ','=','=','=','=','=','+','=','x',
	//' ',' ','|',' ',' ',' ','|',' ',' ',
	//' ',' ','e',' ',' ',' ','|',' ',' ',
	//' ','/','|','\\',' ',' ','|',' ',' ',
	//' ',' ','|',' ',' ',' ','|',' ',' ',
	//' ','/',' ','\\',' ',' ','|',' ',' ',
	//' ',' ',' ',' ',' ',' ','|',' ',' ',
	//' ',' ',' ',' ',' ',' ','|',' ',' ',
	//' ',' ',' ',' ',' ',' ','|',' ',' ',
	//' ',' ',' ',' ',' ','/','|','\\',' ',
	//' ',' ',' ',' ',' ',' ',' ',' ',' '
	//};

public:
	Hangman(){
		setup();
	}
	~Hangman() {}

	void setup()
	{
		if (wordMap.empty())
			readFromFile();

		topic = randomTopic();
		word = randomWord(topic);

		clearScreen();
		wrongGuessCount = 1;
	}

	void run();

	void readFromFile();
	void printWords(){
		for (auto pair : wordMap){
			for(auto item : pair.second)
				std::cout << item << "\n";
		}
	}
	void clearScreen() {
		std::system("cls");
	}

	std::string randomTopic() {
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> number(0, wordMap.size()-1);
		int randTopic = number(rng);
		int count = 0;
		for (auto topic : wordMap) {
			if (count == randTopic)
				return topic.first;
			count++;
		}
	}

	std::string randomWord(std::string topic)
	{
		std::mt19937 rng(std::random_device{}());
		std::uniform_int_distribution<int> number(0, wordMap[topic].size() - 1);
		return wordMap[topic].at(number(rng));
	}

	void printGameState(){
		for (int row = 0; row < 14; ++row)
		{
			for (int column = 0; column < 9; ++column)
			{
				std::cout << std::setw(2) << hangman[row][column];
			}
			std::cout << "\n";
		}
	}

	void resetHangman() {
		std::memcpy(hangman, blankState, sizeof(hangman));
	}

	void updateGameState(int& wrong, bool &lost){
		//Rekursio vaan testimielessä. Normaalipelissä mennään kohta kerrallaan eikä rekursiota tarvita.

		if (wrongGuessCount == 1)
			hangman[4][3] = '|';
		if (wrongGuessCount == 2) {
			//updateGameState(--wrongGuessCount,lost);
			hangman[5][3] = 'O';
		}
		if (wrongGuessCount == 3) {
			//updateGameState(--wrongGuessCount,lost);
			hangman[6][3] = '|';
		}
		if (wrongGuessCount == 4) {
			//updateGameState(--wrongGuessCount,lost);
			hangman[6][2] = '/';
		}
		if (wrongGuessCount == 5) {
			//updateGameState(--wrongGuessCount,lost);
			hangman[6][4] = '\\';
		}
		if (wrongGuessCount == 6) {
			//updateGameState(--wrongGuessCount,lost);
			hangman[7][2] = '/';
		}
		if (wrongGuessCount == 7){
			//updateGameState(--wrongGuessCount,lost);
			hangman[7][4] = '\\';
			lost = true;
		}

		wrong++;
	}

	char readValidGuess();
	bool matchFound(std::string word, char guess, std::vector<int>& indexes);
	bool won(std::string word1, std::vector<char> word2);
};

