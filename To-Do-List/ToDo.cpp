#include "ToDo.h"
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

void ToDo::run()
{
	//readFromFile();

	std::cout << "---------------TASKMASTER 2026 mk 1.0---------------------------------\n\n";
	while (isRunning)
	{
		printMenu();
		makeSelection(processMenu());
	}

	writeToFile();
}

void ToDo::printMenu()
{
	std::cout << "1) ADD NEW TASK\n";
	std::cout << "2) LIST ACTIVE TASKS\n";
	std::cout << "3) LIST COMPLETED TASKS\n";
	std::cout << "4) MARK TASK AS IN PROGRESS\n";
	std::cout << "5) MARK TASK AS COMPLETED\n";
	std::cout << "6) REMOVE TASK\n";
	std::cout << "7) QUIT\n";
}

int ToDo::processMenu()
{
	std::string line;
	while (true)
	{
		std::cout << "Select option: ";

		if (!std::getline(std::cin, line))
		{
			std::cin.clear();
			std::cout << "Invalid input. (Pressed some weird macro)\n";
			continue;
		}

		if (line.empty() || line.find_first_not_of(" \t") == std::string::npos)
			continue;

		try
		{
			size_t pos;
			int selection = std::stoi(line, &pos);
			if (pos == line.size() && selection >= 1 && selection <= 7) //Input was int
				return selection;

			std::cout << "Selection was out of range.\n";
		}
		catch(...)
		{
			std::cout << "Please enter a valid number.\n";
		}
	}
}

void ToDo::makeSelection(int selection)
{
	switch(selection)
	{
	case 1:
		addTask();
		break;
	case 2:
		printActiveTasks();
		break;
	case 3:
		printCompletedTasks();
		break;
	case 4:
		markAsInProgress();
		break;
	case 5:
		markAsCompleted();
		break;
	case 6:
		removeTask();
		break;
	case 7:
		isRunning = false;
		break;
	}
}

void ToDo::addTask()
{
	std::cout << "\n\nInsert a new task.\n\n";

	std::string task;
	std::getline(std::cin, task);
	activeTasks[activeId] = Item{ activeId, task, Progress::NotStarted };
	activeId++;
}

void ToDo::printActiveTasks()
{
	if (activeTasks.empty())
	{
		std::cout << "No active tasks!\n";
		return;
	}

	for (auto i : activeTasks)
		std::cout << i.second.id << ": " << i.second.task << " (" << enumToString(i.second.progress) << ")\n";
}

void ToDo::printCompletedTasks()
{
	if (completedTasks.empty())
	{
		std::cout << "No completed tasks!\n";
		return;
	}

	for (auto i : completedTasks)
		std::cout << i.second.id << ": " << i.second.task << " (" << enumToString(i.second.progress) << ")\n";
}

void ToDo::removeTask()
{
	if (activeTasks.empty())
	{
		std::cout << "\nNo active tasks!\n";
		return;
	}
	printActiveTasks();

	std::cout << "\nSelect task to be deleted: ";

	std::string line;
	while (true)
	{
		if (!std::getline(std::cin, line))
		{
			std::cin.clear();
			std::cout << "Invalid input. (Pressed some weird macro)\n";
			continue;
		}
		try
		{
			int id = std::stoi(line);
			if (id > 0 && id <= activeId) //Input was int
			{
				if (activeTasks.contains(id))
				{
					activeTasks.erase(id);
					break;
				}
			}
			std::cout << "Selection was invalid: " << id << ", Please select an option that pleases you.\n";
		}
		catch (...)
		{
			std::cout << "Please enter a valid ID.\n";
		}
	}
}

void ToDo::markAsInProgress()
{
	if (activeTasks.empty())
	{
		std::cout << "\nNo active tasks!\n";
		return;
	}
	printActiveTasks();

	std::cout << "Mark which task as in prgress? ";

	std::string line;
	while (true)
	{
		if (!std::getline(std::cin, line))
		{
			std::cin.clear();
			std::cout << "Invalid input. (Pressed some weird macro)\n";
			continue;
		}
		try
		{
			int id = std::stoi(line);
			if (id > 0 && id <= activeId) //Input was int
			{
				if (activeTasks.contains(id))
				{
					activeTasks[id].progress = Progress::InProgress;
					break;
				}
			}
			else
				std::cout << "Selection was invalid: " << id << ", Please select an option that pleases you.\n";
		}
		catch (...)
		{
			std::cout << "Please enter a valid ID.\n";
		}
	}
}

void ToDo::markAsCompleted()
{
	if (activeTasks.empty())
	{
		std::cout << "\nNo active tasks!\n";
		return;
	}
	printActiveTasks();

	std::cout << "Mark which task as complete? ";

	std::string line;
	while (true)
	{
		if (!std::getline(std::cin, line))
		{
			std::cin.clear();
			std::cout << "Invalid input. (Pressed some weird macro)\n";
			continue;
		}
		try
		{
			int id = std::stoi(line);
			if (id > 0 && id <= activeId) //Input was int
			{
				if (activeTasks.contains(id))
				{
					activeTasks[id].progress = Progress::Completed;
					moveToCompleted(id);
					break;
				}
			}
			std::cout << "Selection was invalid: " << id << ", Please select an option that pleases you.\n";
		}
		catch (...)
		{
			std::cout << "Please enter a valid ID.\n";
		}
	}
}

void ToDo::moveToCompleted(int id)
{
	completedTasks[completedId] = activeTasks[id];
	activeTasks.erase(id);
	completedId++;
	std::cout << "Task " << id << "completed!\n\n";
}

std::string ToDo::enumToString(Progress p)
{
	switch (p)
	{
	case Progress::NotStarted:
		return "Not Started";
		break;
	case Progress::InProgress:
		return "In Progress";
		break;
	case Progress::Completed:
		return "Completed";
		break;
	case Progress::Abandoned:
		return "Abandoned";
		break;
	default:
		return "0";
		break;
	}
}

Progress ToDo::stringToEnum(std::string s)
{
	if(s == "Not Started")
		return Progress::NotStarted;
	if(s == "In Progress")
		return Progress::InProgress;
	if (s == "Completed")
		return Progress::Completed;
	if (s == "Abandoned")
		return Progress::Abandoned;

	return Progress::Abandoned;//Something went wrong because we don't use Abandoned at the present.
}

void ToDo::readFromFile()
{
	std::string filename("TASKMASTER.txt");
	std::fstream file(filename, std::ios::in | std::ios::out);
	if (!file.is_open())
	{
		std::cout << "Couldn't open file: " << filename << "Creating it out of thin air!\n";
		file.open(filename, std::ios::out);
	}
	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty() || line[0] == '-') continue;

		std::stringstream ss(line);
		Item i;
		
		std::string temp;
		std::getline(ss, temp, ';');
		try { i.id = std::stoi(temp); }catch (...) {}

		std::getline(ss, i.task, ';');
		
		std::getline(ss, temp, ';');
		try { i.progress = stringToEnum(temp); }catch (...) {}

	}
}

bool ToDo::writeToFile()
{
	std::string filename("TASKMASTER.txt");
	std::fstream file(filename, std::ios::in | std::ios::out);
	if (!file.is_open())
	{
		std::cout << "Couldn't open file: " << filename << "Creating it out of thin air!\n";
		file.open(filename, std::ios::out);
	}
	if (!activeTasks.empty())
	{
		file << "---------ACTIVE TASKS------------\n";
		for (const auto& i : activeTasks)
		{
			file << i.second.id << ";" << i.second.task << ";" << enumToString(i.second.progress) << "\n";
		}
	}

	if (!completedTasks.empty())
	{
		file << "---------COMPLETED TASKS------------\n";
		for (const auto& i : completedTasks)
		{
			file << i.second.id << ";" << i.second.task << ";" << enumToString(i.second.progress) << "\n";
		}
	}
	return true;
}