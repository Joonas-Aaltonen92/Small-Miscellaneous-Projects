#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include "Game.h"
#include "json.hpp"

namespace {
	std::string equipmentSlotToString(EquipmentSlot slot) {
		switch (slot) {
			case EquipmentSlot::HEAD: return "head";
			case EquipmentSlot::CHEST: return "chest";
			case EquipmentSlot::WAIST: return "waist";
			case EquipmentSlot::LEGS: return "legs";
			case EquipmentSlot::FEET: return "feet";
			case EquipmentSlot::ARMS: return "arms";
			case EquipmentSlot::HAND: return "hand";
			case EquipmentSlot::RING: return "ring";
			case EquipmentSlot::AMULET: return "amulet";
			default: return "unknown";
		}
	}

	nlohmann::json serializeEquipment(const std::unordered_map<EquipmentSlot, std::vector<std::string>>& equipment) {
		nlohmann::json jsonData;

		for (const auto& [slot, items] : equipment) {
			jsonData[equipmentSlotToString(slot)] = items;
		}

		return jsonData;
	}
	
	nlohmann::json serializeActorStats(const ActorStats& stats) {
		nlohmann::json jsonData;

		jsonData["baseStats"] = stats.baseStats;
		jsonData["growthRates"] = stats.growthRates;

		return jsonData;
	}
	
	nlohmann::json serializePlayer(const PlayerState& player) {
		nlohmann::json jsonData;
		jsonData["name"] = player.name;
		jsonData["classId"] = player.classId;
		jsonData["level"] = player.level;
		jsonData["skillPoints"] = player.skillPoints;
		jsonData["experience"] = player.experience;
		jsonData["gold"] = player.gold;
		jsonData["walletSize"] = player.walletSize;
		jsonData["inventory"] = player.inventory;
		jsonData["stats"] = serializeActorStats(player.stats);
		jsonData["equipped"] = serializeEquipment(player.equipped);

		return jsonData;
	}

	nlohmann::json serializeRoomState(const RoomState& room) {
		nlohmann::json jsonData;

		jsonData["visited"] = room.visited;

		jsonData["deadEnemies"] = room.deadEnemies;
		jsonData["emptyContainers"] = room.emptiedContainers;
		jsonData["openedDoors"] = room.openedDoors;
		jsonData["departedNPCs"] = room.departedNPCs;
		jsonData["departedMerchants"] = room.departedMerchants;
		jsonData["itemsPickedUp"] = room.itemsPickedUp;

		jsonData["spawnedEnemies"] = room.spawnedEnemies;
		jsonData["spawnedContainers"] = room.spawnedContainers;
		jsonData["spawnedDoors"] = room.spawnedDoors;
		jsonData["spawnedNPCs"] = room.spawnedNPCs;
		jsonData["spawnedMerchants"] = room.spawnedMerchants;
		jsonData["spawnedItems"] = room.spawnedItems;

		return jsonData;
	}

	EquipmentSlot stringToEquipmentSlot(const std::string& str) {
		static const std::unordered_map<std::string, EquipmentSlot> stringToEquipmentSlotMap = {
			{"head", EquipmentSlot::HEAD},
			{"chest", EquipmentSlot::CHEST},
			{"waist", EquipmentSlot::WAIST},
			{"legs", EquipmentSlot::LEGS},
			{"feet", EquipmentSlot::FEET},
			{"arms", EquipmentSlot::ARMS},
			{"hand", EquipmentSlot::HAND},
			{"ring", EquipmentSlot::RING},
			{"amulet", EquipmentSlot::AMULET},
			{"unknown", EquipmentSlot::UNKNOWN}
		};
		const auto it = stringToEquipmentSlotMap.find(str);
		return it != stringToEquipmentSlotMap.end() ? it->second : EquipmentSlot::UNKNOWN;
	}

	std::unordered_map<EquipmentSlot, std::vector<std::string>> deserializeEquipment(const nlohmann::json& jsonData) {
		std::unordered_map<EquipmentSlot, std::vector<std::string>> equipment;
		
		for (const auto& [slotName, items] : jsonData.items()) {
			EquipmentSlot slot = stringToEquipmentSlot(slotName);
			if (slot == EquipmentSlot::UNKNOWN)
				continue;

			equipment.emplace(slot, items.get<std::vector<std::string>>());
		}

		return equipment;
	}

	ActorStats deserializeActorStats(const nlohmann::json& jsonData) {
		ActorStats stats;

		if (jsonData.contains("baseStats")) {
			stats.baseStats = jsonData["baseStats"].get<decltype(stats.baseStats)>();
		}
		if (jsonData.contains("growthRates")) {
			stats.growthRates = jsonData["growthRates"].get<decltype(stats.growthRates)>();
		}

		return stats;
	}
	PlayerState deserializePlayerState(const nlohmann::json& jsonData) {
		PlayerState player;

		player.name = jsonData.value("name", "unknown");
		player.classId = jsonData.value("classId", "unknown class");
		player.level = jsonData.value("level", 0);
		player.skillPoints = jsonData.value("skillPoints", 0);
		player.experience = jsonData.value("experience", 0);
		player.gold = jsonData.value("gold", 0);
		player.walletSize = jsonData.value("walletSize", 0);
		if (jsonData.contains("stats"))
			player.stats = deserializeActorStats(jsonData["stats"]);
		if (jsonData.contains("inventory"))
			player.inventory = jsonData.value("inventory", std::unordered_map<std::string, int>{});
		if (jsonData.contains("equipped")) {
			player.equipped = deserializeEquipment(jsonData["equipped"]);
		}
		return player;
	}


	std::unordered_map < std::string, RoomState> deserializeRoomStates(const nlohmann::json& jsonData) {
		
		std::unordered_map<std::string, RoomState> rooms;
		rooms.reserve(jsonData.size());

		for (const auto& [id, definition] : jsonData.items()) {
			RoomState room;
			
			room.visited = definition.value("visited", false);
			
			room.deadEnemies = definition.value("deadEnemies", std::unordered_set<std::string>{});
			room.emptiedContainers = definition.value("emptyContainers", std::unordered_set<std::string>{});
			room.departedNPCs = definition.value("departedNPCs", std::unordered_set<std::string>{});
			room.departedMerchants = definition.value("departedMerchants", std::unordered_set<std::string>{});
			room.openedDoors = definition.value("openedDoors", std::unordered_set<std::string>{});
			room.itemsPickedUp = definition.value("itemsPickedUp", std::unordered_map<std::string, int>{});
			
			room.spawnedEnemies = definition.value("spawnedEnemies", std::unordered_set<std::string>{});
			room.spawnedContainers = definition.value("spawnedContainers", std::unordered_set<std::string>{});
			room.spawnedDoors = definition.value("spawnedDoors", std::unordered_set<std::string>{});
			room.spawnedNPCs = definition.value("spawnedNPCs", std::unordered_set<std::string>{});
			room.spawnedMerchants = definition.value("spawnedMerchants", std::unordered_set<std::string>{});
			room.spawnedItems = definition.value("spawnedItems", std::unordered_map<std::string,int>{});

			rooms.emplace(id,std::move(room));
		}
		return rooms;
	}
}

void Game::loadDatabases() {
	_itemDatabase.loadFromJson("items.json");
	_actorDatabase.loadActors();
	_roomDatabase.loadFromJson("rooms.json");
	_playerClassDatabase.loadFromJson("playerClasses.json");
}

bool Game::saveGame(const std::string& filename) {
	nlohmann::json root;

	root["player"] = serializePlayer(_gameState.player);
	for (const auto& [id, room] : _gameState.rooms) {
		root["rooms"][id] = serializeRoomState(room);
	}

	std::ofstream file(filename);

	if (!file) {
		std::cerr << "Could not open file for saving: " << filename << std::endl;
		return false;
	}

	file << root.dump(4);

	if (!file) {
		std::cerr << "Could not write save file: " << filename << "\n";
		return false;
	}

	return true;
}

bool Game::loadGame(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "No save file found. Start a new game instead.\n";
		return false;
	}


	try {
		nlohmann::json jsonData;
		file >> jsonData;

		_gameState.player = deserializePlayerState(jsonData.at("player"));
		_gameState.rooms = deserializeRoomStates(jsonData.at("rooms"));

		if (jsonData.contains("currentRoom")) {
			_gameState.player.currentRoom = jsonData["currentRoom"];
		}
		else {
			_gameState.player.currentRoom = "town_square_01";
		}

		_inGame = true;
		std::cout << "Game loaded successfully.\n";
		return true;

	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while loading save file: " << e.what() << std::endl;
		return false;
	}


	return true;
}


void Game::mainMenu()
{
	while (_running)
	{
		std::cout << "\n=== Geemu ===\n";
		std::cout << "1. New Game\n";
		std::cout << "2. Load Game\n";
		std::cout << "3. Quit\n";
		std::cout << "> ";

		char input;
		std::cin >> input;

		switch (input)
		{
		case '1':
			newGame();
			if (_inGame)
				gameLoop();
			break;

		case '2':
			if (loadGame("save.json"))
			{
				_inGame = true;
				gameLoop();
			}
			break;

		case '3':
			_running = false;
			break;

		default:
			std::cout << "Invalid selection.\n";
			break;
		}
	}
}
void Game::newGame()
{
	_gameState = GameState();
	PlayerState& player = _gameState.player;

	std::cout << "\nEnter Your Name: ";
	std::cin >> player.name;
	while (true) {


		std::cout << "\nChoose Your Class:\n";

		std::cout << "1. Warrior\n";
		std::cout << "2. Rogue\n";
		std::cout << "3. Mage\n";
		std::cout << "> ";

		char input;
		std::cin >> input;

		switch (input) {
		case '1':
			player.classId = "warrior";
			break;
		case '2':
			player.classId = "rogue";
			break;
		case '3':
			player.classId = "mage";
			break;
		default:
			std::cout << "Invalid class selection.\n";
			continue;
		}

		//Make sure the class actually exists
		const PlayerClassDefinition* playerClass = _playerClassDatabase.find(player.classId);
		if (playerClass == nullptr) {
			std::cout << "The selected class is unavailable. Please choose again.\n";
			continue;
		}

		//Valid class selected
		player.stats = playerClass->baseStats;
		player.growthRates = playerClass->baseStats.growthRates;
		player.inventory = playerClass->startingInventory;

		break;
	}

	player.level = 1;
	player.skillPoints = 0;
	player.experience = 0;
	player.gold = 20;
	
	player.currentRoom = "town_square_01";
	_gameState.rooms[player.currentRoom].visited = true;

	_inGame = true;

	std::cout << "\nGet ready for some adventuring, " << player.name << "!\n";
}

void Game::gameLoop()
{
	while (_inGame && _running)
	{
		displayCurrentRoom();

		std::cout << "\n> ";

		char input;
		std::cin >> input;

		handleGameInput(input);
	}
}
void Game::handleGameInput(char c)
{
	const RoomDefinition* currentRoom = _roomDatabase.find(_gameState.player.currentRoom);

	if (currentRoom == nullptr) {
		std::cout << "Error: Current room not found.\n";
		return;
	}

	Exits direction = Exits::UNKNOWN;

	switch (c)
	{
	case 'n':
		direction = Exits::NORTH;
		break;

	case 'e':
		direction = Exits::EAST;
		break;

	case 's':
		direction = Exits::SOUTH;
		break;

	case 'w':
		direction = Exits::WEST;
		break;

	case 'u':
		direction = Exits::UP;
		break;
	case 'd':
		direction = Exits::DOWN;
		break;
	case 'i':
		displayInventory();
		return;
	case 'q':
		saveGame("save.json");
		_inGame = false;
		return;
	default:
		std::cout << "Unknown command.\n";
		return;
	}

	auto exit = currentRoom->exits.find(direction);
	if (exit == currentRoom->exits.end()) {
		std::cout << "You cannot go that way.\n";
		return;
	}

	_gameState.player.currentRoom = exit->second;

	RoomState& roomState = _gameState.rooms[exit->second];
	roomState.visited = true;
}

void Game::displayCurrentRoom() const
{
	const PlayerState& player = _gameState.player;
	const RoomDefinition* room = _roomDatabase.find(player.currentRoom);

	if (room == nullptr) {
		std::cout << "\nError: Current room not found.";
		return;
	}
	std::cout << "\n===" << room->name << "===\n";
	std::cout << room->description << "\n";

	if (!room->exits.empty()) {
		std::cout << "\nExits: ";
		bool first = true;

		for (const auto& [exit, destination] : room->exits) {
			if (!first)
				std::cout << ", ";
			switch (exit) {
			case Exits::NORTH:
				std::cout << "North";
				break;
			case Exits::EAST:
				std::cout << "East";
				break;
			case Exits::SOUTH:
				std::cout << "South";
				break;
			case Exits::WEST:
				std::cout << "West";
				break;
			case Exits::UP:
				std::cout << "Up";
				break;
			case Exits::DOWN:
				std::cout << "Down";
				break;
			default:
				break;
			}
			first = false;
		}
		std::cout << "\n";
	}
}

void Game::displayInventory() {
	bool inventoryActive = true;

	while (inventoryActive) {
		std::cout << "\n====INVENTORY====\n\n";

		if (_gameState.player.inventory.empty()) {
			std::cout << "Your inventory is empty :DDD\n";
			std::cout << "\nPress 0 to return :DDD\n>";

			char input;
			std::cin >> input;
			if (input == '0')
				inventoryActive = false;

			continue;
		}

		std::vector<std::string> itemIDs;
		int index = 1;
		for (const auto& [itemId, quantity] : _gameState.player.inventory) {
			const ItemDefinition* item = _itemDatabase.find(itemId);

			if (item == nullptr)
				continue;

			itemIDs.push_back(itemId);

			std::cout << index << ". " << item->name;

			if (quantity > 1)
				std::cout << " x" << quantity;

			std::cout << '\n';
			++index;
		}

		std::cout << "0. Back\n";
		std::cout << "Select an item: ";

		int selection;
		std::cin >> selection;

		if (selection == 0) {
			inventoryActive = false;
			continue;
		}

		if (selection < 1 || selection > static_cast<int>(itemIDs.size())){
			std::cout << "Invalid Selection :DDD\n";
			continue;
		}

		const std::string& itemID = itemIDs[selection - 1];
		const ItemDefinition* item = _itemDatabase.find(itemID);

		if (item == nullptr) {
			std::cout << "Item not found :DDD\n";
			continue;
		}
		std::cout << "\n===" << item->name << "===\n";
		std::cout << item->description << '\n';
		std::cout << "Value: " << item->value << " Gold Coins\n";

		switch (item->type) {
		case ItemType::EQUIPMENT:
			std::cout << "Type: Equipment\n";
			break;
		case ItemType::CONSUMABLE:
			std::cout << "Type: Consumable\n";
			break;
		case ItemType::TRINKET:
			std::cout << "Type: Trinket\n";
			break;
		case ItemType::KEYITEM:
			std::cout << "Type: Important Item\n";
			break;
		case ItemType::KEY:
			std::cout << "Type: Key\n";
			break;
		default:
			std::cout << "Type: Unknown :DDD\n";
			break;
		}

		std::cout << "\nPress 0 to return to inventory.\n>";
		
		int input;
		std::cin >> input;
		if (input == 0)
			continue;
	}
}

void Game::levelUpPlayer() {
	PlayerState& player = _gameState.player;

	++player.level;

	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> roll(1, 100);

	constexpr int HPMP_PER_GROWTHSTAT = 5;

	auto applyGrowth = [&](GrowthStat growthStat, CombatStat combatStat) {
		const int growthIndex = static_cast<int>(growthStat);
		const int combatIndex = static_cast<int>(combatStat);

		int growthRate = static_cast<int>(player.growthRates[growthIndex]);

		int statIncrease = growthRate / 100;
		int remainder = growthRate % 100;

		if (roll(rng) <= remainder)
			++statIncrease;

		player.stats.baseStats[combatIndex] += statIncrease;
		};

	applyGrowth(GrowthStat::VITALITY, CombatStat::MAXHP);
	applyGrowth(GrowthStat::WISDOM, CombatStat::MAXMP);
	applyGrowth(GrowthStat::STRENGTH, CombatStat::POWER);
	applyGrowth(GrowthStat::ENDURANCE, CombatStat::FORTITUDE);
	applyGrowth(GrowthStat::INTELLIGENCE, CombatStat::SORCERY);
	applyGrowth(GrowthStat::RESOLVE, CombatStat::WILLPOWER);
	applyGrowth(GrowthStat::AGILITY, CombatStat::SPEED);
	applyGrowth(GrowthStat::FATE, CombatStat::LUCK);

	auto calculateGrowthIncrease = [&](GrowthStat growthStat) {
		const int growthIndex = static_cast<int>(growthStat);
		int growthRate = static_cast<int>(player.growthRates[growthIndex]);

		int increase = growthRate / 100;
		int remainder = growthRate % 100;

		if (roll(rng) <= remainder)
			++increase;
		return increase;
		};
	const int vitalityIncrease = calculateGrowthIncrease(GrowthStat::VITALITY);
	const int wisdomIncrease = calculateGrowthIncrease(GrowthStat::WISDOM);

	player.stats.baseStats[static_cast<int>(CombatStat::MAXHP)] += vitalityIncrease * HPMP_PER_GROWTHSTAT;
	player.stats.baseStats[static_cast<int>(CombatStat::MAXMP)] += wisdomIncrease * HPMP_PER_GROWTHSTAT;

	player.stats.baseStats[static_cast<int>(CombatStat::HP)] = player.stats.baseStats[static_cast<int>(CombatStat::MAXHP)];
	player.stats.baseStats[static_cast<int>(CombatStat::MP)] = player.stats.baseStats[static_cast<int>(CombatStat::MAXMP)];

	std::cout << "\n====LEVEL UP====\n\n";
	std::cout << "You've reached level " << player.level << "!\n";

	bool choosingGrowth = true;

	while (choosingGrowth) {
		std::cout << "\nChoose a growth stat to improve:\n";
		std::cout << "1. Vitality (" << player.growthRates[static_cast<int>(GrowthStat::VITALITY)] << "%\n";
		std::cout << "2. Wisdom (" << player.growthRates[static_cast<int>(GrowthStat::WISDOM)] << "%\n";
		std::cout << "3. Strength (" << player.growthRates[static_cast<int>(GrowthStat::STRENGTH)] << "%\n";
		std::cout << "4. Endurance (" << player.growthRates[static_cast<int>(GrowthStat::ENDURANCE)] << "%\n";
		std::cout << "5. Intelligence (" << player.growthRates[static_cast<int>(GrowthStat::INTELLIGENCE)] << "%\n";
		std::cout << "6. Resolve (" << player.growthRates[static_cast<int>(GrowthStat::RESOLVE)] << "%\n";
		std::cout << "7. Agility (" << player.growthRates[static_cast<int>(GrowthStat::AGILITY)] << "%\n";
		std::cout << "8. Fate (" << player.growthRates[static_cast<int>(GrowthStat::FATE)] << "%\n";

		std::cout << "> ";

		int choice;
		std::cin >> choice;

		GrowthStat selectedGrowth = GrowthStat::UNKNOWN;

		switch (choice) {
		case 1:
			selectedGrowth = GrowthStat::VITALITY;
			break;
		case 2:
			selectedGrowth = GrowthStat::WISDOM;
			break;
		case 3:
			selectedGrowth = GrowthStat::STRENGTH;
			break;
		case 4:
			selectedGrowth = GrowthStat::ENDURANCE;
			break;
		case 5:
			selectedGrowth = GrowthStat::INTELLIGENCE;
			break;
		case 6:
			selectedGrowth = GrowthStat::RESOLVE;
			break;
		case 7:
			selectedGrowth = GrowthStat::AGILITY;
			break;
		case 8:
			selectedGrowth = GrowthStat::FATE;
			break;
		default:
			std::cout << "Invalid selection. Please choose a growth stat.\n";
		}

		const int growthIndex = static_cast<int>(selectedGrowth);
		player.growthRates[growthIndex] += 10.0f;
		std::cout << "\nGrowth rate increased to " << player.growthRates[growthIndex] << "%.\n";

		choosingGrowth = false;
	}
}

void Game::useItem(const std::string& itemId) {
	const ItemDefinition* item = _itemDatabase.find(itemId);
	if (item == nullptr) {
		std::cout << "Item not found.\n";
		return;
	}

	auto inventoryItem = _gameState.player.inventory.find(itemId);
	if (inventoryItem == _gameState.player.inventory.end()) {
		std::cout << "You don't have that item.\n";
		return;
	}

	switch (item->type) {
	case ItemType::EQUIPMENT:
	{

		if (!item->isEquippable()) {
			std::cout << "You cannot equip this item.\n";
			return;
		}
		for (size_t i = 0; i < COMBAT_STAT_COUNT; ++i) {
			_gameState.player.stats.baseStats[i] += item->modifiers.flatModifiers[i];
		}

		for (const auto& [slot, cost] : item->slotCosts) {
			if (cost > 0)
				_gameState.player.equipped[slot].push_back(itemId);
		}

		std::cout << "You equip the " << item->name << ".\n";

		break;
	}
	case ItemType::CONSUMABLE:
	{
		for (size_t i = 0; i < COMBAT_STAT_COUNT; ++i) {
			_gameState.player.stats.baseStats[i] += item->modifiers.flatModifiers[i];
		}

		int& hp = _gameState.player.stats.baseStats[static_cast<size_t>(CombatStat::HP)];
		const int maxHP = _gameState.player.stats.baseStats[static_cast<size_t>(CombatStat::MAXHP)];

		int& mp = _gameState.player.stats.baseStats[static_cast<size_t>(CombatStat::MP)];
		const int maxMP = _gameState.player.stats.baseStats[static_cast<size_t>(CombatStat::MAXMP)];

		hp = std::min(hp, maxHP);
		mp = std::min(mp, maxMP);

		--inventoryItem->second;

		if (inventoryItem->second <= 0)
			_gameState.player.inventory.erase(inventoryItem);

		std::cout << "You used the " << item->name << ".\n";
		break;
	}
	case ItemType::KEY:
		std::cout << "You need to choose a target to unlock.\n";
		break;
	case ItemType::KEYITEM:
		std::cout << "You cannot use that item right now.\n";
		break;
	case ItemType::TRINKET:
		std::cout << "This item has no use.\n";
		break;

	default:
		std::cout << "You cannot use that item.\n";
		break;
	}
}

void Game::run()
{
	loadDatabases();
	mainMenu();
}