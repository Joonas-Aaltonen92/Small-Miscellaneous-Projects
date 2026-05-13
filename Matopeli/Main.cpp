#include <iostream>
#include <windows.h>    // for SetConsoleOutputCP
#include <string>
#include "Matopeli.h"

int main()
{
	// Make console interpret output as UTF-8 (Windows Terminal loves this)
	SetConsoleOutputCP(CP_UTF8);   // 65001

	Snake snek;
	Matopeli mato(snek);
	mato.setup();
	mato.Run();
	std::cin.get();
	return 0;
}