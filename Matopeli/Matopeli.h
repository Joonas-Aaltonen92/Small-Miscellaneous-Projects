#pragma once
#include <iostream>
#include <string>
#include <iomanip>
#include <chrono>
#include "Snake.h"
#include <vector>
#include <random>
#include <thread>
#include <conio.h>

using namespace std::chrono_literals;

class Matopeli {
	char gameArea[20][20] = {
		'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','*',
		'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'
	};

	std::vector<Vec2> emptySpaces;

	Snake& snake;
	const char food = '0';
	bool running = true;

public:
	Matopeli(Snake& snek) : snake(snek) {}

	void printArea() {
		for (int row = 0; row < 20; ++row) {
			for (int column = 0; column < 20; ++column)
				std::cout << std::setw(3)<< gameArea[row][column];
			std::cout << "\n";
		}
	}

	void setup() {
		if (gameArea[snake.head->position.x][snake.head->position.y] == ' ')
			gameArea[snake.head->position.x][snake.head->position.y] = snake.head->bodySymbol;
		if (gameArea[snake.neck->position.x][snake.neck->position.y] == ' ')
			gameArea[snake.neck->position.x][snake.neck->position.y] = snake.neck->bodySymbol;

		spawnFood();
	}

	void Run() {
		auto moveInterval = 150ms;

		while (running) {
			if (_kbhit()) {
				char  c = _getch();
				snake.changeDirection(c);
			}
			moveSnakeHeadToDirection(snake.head,snake.head->direction);
			clearScreen();
			printArea();

			std::this_thread::sleep_for(moveInterval);
		}
	}

	void moveSnakeHeadToDirection(Body* body, Vec2 direction) {
		if (body == nullptr) //Shouldn't be null because only used for the head
			return;
		Vec2 nextPos = body->position + direction;
		Vec2 tailPos = snake.findTailNode(body)->position;
		Vec2 oldPos = body->position;
		bool grew = false;
		bool eatingTail = (nextPos == tailPos);
		
		if (nextPos == body->tailNode->position)//Head shouldn't be able to move to previous position
			return;
		else if (gameArea[nextPos.x][nextPos.y] == '*'){//Hit wall
			passWall(body, direction);
			gameArea[body->position.x][body->position.y] = body->bodySymbol;
			moveSnakeBody(body->tailNode, oldPos);
		}
		else if (gameArea[nextPos.x][nextPos.y] == body->tailNode->bodySymbol) {//Hit own tail
			if (!eatingTail) {
				//End Game
				loseGame();
				return; //Early return so the game doesn't clear.
			}
		}
		else if (gameArea[nextPos.x][nextPos.y] == food) {//Hit food
			spawnFood(); // winning condition can be made here
			snake.addNewBodyPart(snake.findTailNode(body));
			body->position = nextPos;
			gameArea[body->position.x][body->position.y] = body->bodySymbol;
			moveSnakeBody(body->tailNode, oldPos);
			grew = true;
		}
		if (gameArea[nextPos.x][nextPos.y] == ' ' || eatingTail) {
			body->position = nextPos;
			gameArea[body->position.x][body->position.y] = body->bodySymbol;
			moveSnakeBody(body->tailNode, oldPos);
		}
		if (!grew && !eatingTail)
			clearTailSymbolFromGameArea(tailPos);
	}

	void clearTailSymbolFromGameArea(Vec2 position) {
		gameArea[position.x][position.y] = ' ';
	}

	void moveSnakeBody(Body* body, Vec2 newPos) {
		if (body == nullptr)
			return;
		Vec2 oldPos = body->position;
		body->position = newPos;
		gameArea[body->position.x][body->position.y] = body->bodySymbol;
		if (body->tailNode != nullptr) {
			moveSnakeBody(body->tailNode, oldPos);
		}
	}

	void spawnFood() {
		if (checkWinCondition()) {
			winGame();
			return;
		}
		
		//Check for all empty spaces, then choose from them randomly.
		emptySpaces.clear();
		for (int row = 1; row < 20; ++row) {
			for (int column = 1; column < 20; ++column) {
				if (gameArea[row][column] == ' ')
					emptySpaces.push_back(Vec2(row,column));
			}
		}

		if (!emptySpaces.empty()) {
			if (emptySpaces.size() == 1) {
				gameArea[emptySpaces.at(0).x][emptySpaces.at(0).y] = food;
				return;
			}
			std::mt19937 rng(std::random_device{}());
			std::uniform_int_distribution<int> position(0, emptySpaces.size() - 1);
			int random = position(rng);
			gameArea[emptySpaces.at(random).x][emptySpaces.at(random).y] = food;
		}
	}

	void clearScreen() {
		std::system("cls");
	}

	bool checkWinCondition() {
		for (int row = 1; row < 20; ++row) {
			for (int column = 1; column < 20; ++column) {
				if (gameArea[row][column] == ' ')
					return false;
			}
		}
		return true; //Area completely filled up
	}

	void winGame(){
		clearScreen();
		std::cout << "YOU WON. GOOD FOR YOU.\n";
		running = false;
	}

	void loseGame() {
		running = false;
		clearScreen();
		printArea();
		std::cout << "YOU LOST. WHAT A LOSER, HAHA!.\n";
	}

	void passWall(Body* head, Vec2 direction) {
		
		if (direction.x == 1)//Moving right
			head->position.x = 1;
		else if (direction.x == -1)//Moving left
			head->position.x = 18;
		else if (direction.y == 1)//Moving down
			head->position.y = 1;
		else if (direction.y == -1)//Moving up
			head->position.y = 18;
	}
};
