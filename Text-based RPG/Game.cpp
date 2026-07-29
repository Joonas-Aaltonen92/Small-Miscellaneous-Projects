#include <fstream>
#include <iostream>
#include <vector>
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
			for (const auto& [slotName, items] : jsonData["equipped"].items()) {
				EquipmentSlot slot = stringToEquipmentSlot(slotName);
				player.equipped[slot] = items.get<std::vector<std::string>>();
			}
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

	return true;
}

bool Game::loadGame(const std::string& filename) {
	std::ifstream file(filename);
	if (!file) {
		std::cerr << "Could not open file: " << filename << std::endl;
		return false;
	}

	nlohmann::json jsonData;

	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing save file: " << e.what() << std::endl;
		return false;
	}

	_gameState.player = deserializePlayerState(jsonData["player"]);
	_gameState.rooms = deserializeRoomStates(jsonData["rooms"]);

	return true;
}