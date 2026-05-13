#pragma once
#include<iostream>
#include <unordered_map>

enum Progress
{
	NotStarted = 0,
	InProgress = 1,
	Completed = 2,
	Abandoned = 3
};

struct Item
{
	int id;
	std::string task;
	Progress progress;
};

class ToDo
{
	std::unordered_map<int, Item> activeTasks;
	std::unordered_map<int, Item> completedTasks;

	int activeId = 1;
	int completedId = 1;

	bool isRunning = true;
public:
	void run();
	void printMenu();
	void clearScreen() { std::system("cls"); }
	int processMenu();
	void makeSelection(int selection);
	void addTask();
	void printActiveTasks();
	void printCompletedTasks();
	std::unordered_map<int, Item>& getActiveTasks() { return activeTasks; }
	std::unordered_map<int, Item>& getCompletedTasks() { return completedTasks; }
	void removeTask();
	void markAsInProgress();
	void markAsCompleted();
	void moveToCompleted(int id);

	std::string enumToString(Progress p);
	Progress stringToEnum(std::string s);

	void readFromFile();
	bool writeToFile();
};

