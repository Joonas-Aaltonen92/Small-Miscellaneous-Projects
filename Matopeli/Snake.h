#pragma once
#include <iostream>
#include "Vec2.h"

struct Body {
public:
	const char bodySymbol;
	Body* headNode = nullptr;
	Body* tailNode = nullptr;
	Vec2 position;
	Vec2 direction;
};

class Snake{
public:
	Snake() {
		head = new Body('e');
		neck = new Body('o');
		neck->headNode = head;
		head->tailNode = neck;
		head->position = { 8,8 };
		neck->position = { 7,8 };
		head->direction = { -1,0 };
		//Neck direction is irrelevant, it follows the head
	}
	~Snake() {
		destroySnake(head);
	}
	Body* head = nullptr;
	Body* neck = nullptr;

	Body* addNewBodyPart(Body* tail) {
		Body* body = new Body('o');
		body->headNode = tail;
		tail->tailNode = body;
		body->position = tail->position;
		//Direction is irrelevant.

		return body;
	}

	Body* findTailNode(Body* node) {
		if (node->tailNode == nullptr)
			return node;
		return findTailNode(node->tailNode);
	}

	void destroySnake(Body* body) {
		if (body->tailNode != nullptr)
			destroySnake(body->tailNode);
		std::cout << "Destroying " << body << "\n";
		delete body;
	}

	void changeDirection(const char key) {
		switch (key) {
		case 'w':
			head->direction.x = -1;
			head->direction.y = 0;
			break;
		case 'a':
			head->direction.x = 0;
			head->direction.y = -1;
			break;
		case 's':
			head->direction.x = 1;
			head->direction.y = 0;
			break;
		case 'd':
			head->direction.x = 0;
			head->direction.y = 1;
			break;
		default:
			head->direction = head->direction;
			break;
		}
	}
};
