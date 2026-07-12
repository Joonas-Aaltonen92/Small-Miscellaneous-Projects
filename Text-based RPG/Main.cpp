// Text-Based RPG.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Game.h"
#include "conio.h"

int main()
{
	Game rpg;
	rpg.loadDatabases();

    std::cout << "-------------MAIN MENU-------------\n";
	std::cout << "1) NEW GAME\n";
	std::cout << "2) LOAD GAME\n";
	std::cout << "3) QUIT GAME\n";

	while (1) {
		if (_kbhit()) {
			char c = _getch();
			
			std::cout << c;
			rpg.mainMenuSelect(c);
			if (c == '3')
				break;
		}
	}

	std::cin.get();
}
