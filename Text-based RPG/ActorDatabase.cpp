#include <fstream>
#include <iostream>
#include "ActorDatabase.h"
#include "json.hpp"

namespace {
	CombatStat getCombatStatFromString(const std::string& stat) {
		static const std::unordered_map<std::string, CombatStat> stringToCombatStatMap = {
			{"hp",CombatStat::HP},
			{"maxhp", CombatStat::MAXHP},
			{"mp", CombatStat::MP},
			{"maxmp", CombatStat::MAXMP},
			{"power", CombatStat::POWER},
			{"fortitude", CombatStat::FORTITUDE},
			{"sorcery", CombatStat::SORCERY},
			{"willpower", CombatStat::WILLPOWER},
			{"speed", CombatStat::SPEED},
			{"luck", CombatStat::LUCK},
			{"unknown", CombatStat::UNKNOWN}
		};
		auto it = stringToCombatStatMap.find(stat);
		return it != stringToCombatStatMap.end() ? it->second : CombatStat::UNKNOWN;
	}

	GrowthStat getGrowthStatFromString(const std::string& stat) {
		static const std::unordered_map<std::string, GrowthStat> stringToGrowthStatMap = {
			{"vitality", GrowthStat::VITALITY},
			{"wisdom", GrowthStat::WISDOM},
			{"strength", GrowthStat::STRENGTH},
			{"endurance", GrowthStat::ENDURANCE},
			{"intelligence", GrowthStat::INTELLIGENCE},
			{"resolve", GrowthStat::RESOLVE},
			{"agility", GrowthStat::AGILITY},
			{"fate", GrowthStat::FATE},
			{"unknown", GrowthStat::UNKNOWN}
		};
		auto it = stringToGrowthStatMap.find(stat);
		return it != stringToGrowthStatMap.end() ? it->second : GrowthStat::UNKNOWN;
	}
	ActorStats parseActorStats(const nlohmann::json& jsonStats) {
		ActorStats stats;
		//Setting defaults although all stats should be set up in the json file.
		stats.baseStats.fill(5);
		stats.growthRates.fill(1.1f);
		if (!jsonStats.is_object())
			return stats;
		//Parse base stats
		if (jsonStats.contains("base") && jsonStats["base"].is_object()) {
			auto& base = jsonStats["base"];
			for (auto& [key, value] : base.items()) {
				CombatStat stat = getCombatStatFromString(key);
				if (stat != CombatStat::UNKNOWN && stat != CombatStat::COUNT) {
					stats.baseStats[std::to_underlying(stat)] = value.get<int>();
				}
			}
		}
		//Parse multipliers
		if (jsonStats.contains("growth") && jsonStats["growth"].is_object()) {
			auto& growth = jsonStats["growth"];
			for (auto& [key, value] : growth.items()) {
				GrowthStat stat = getGrowthStatFromString(key);
				if (stat != GrowthStat::UNKNOWN && stat != GrowthStat::COUNT) {
					stats.growthRates[std::to_underlying(stat)] = value.get<float>();
				}
			}
		}
		return stats;
	}
}

bool ActorDatabase::loadNPCFromJson(const std::string& fileName) {
	std::ifstream file(fileName);
	if (!file) {
		std::cerr << "Could not open NPC JSON file: " << fileName << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing NPC JSON file: " << e.what() << std::endl;
		return false;
	}

	_npcs.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		NPCDefinition npc;
		npc.id = id;
		npc.name = definition.value("name", "");
		npc.description = definition.value("description", "");
		if (definition.contains("dialogue") && definition["dialogue"].is_array()) {//Every NPC should have dialogue, but checking just in case
			for (const auto& line : definition["dialogue"]) {
				npc.dialogue.push_back(line.get<std::string>());
			}
		}
		else//NPC did not have dialogue. Not an error, but print a notification to write it in the JSON file
			std::cout << "WARNING: NPC (" << id << ") has no dialogue. Fix the JSON, plz.\n";
		_npcs[id] = npc;
	}
	std::cout << "Loaded " << _npcs.size() << " NPCs from " << fileName << std::endl;
	return true;
}

bool ActorDatabase::loadContainerFromJson(const std::string& filename)
{
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open Container JSON file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing Container JSON file: " << e.what() << std::endl;
		return false;
	}

	_containers.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		ContainerDefinition container;
		container.id = id;
		container.name = definition.value("name", "");
		container.description = definition.value("description", "");
		if (definition.contains("items") && definition["items"].is_object()) {//Should contain items, but checking just in case
			for (const auto& [itemId, quantity] : definition["items"].items()) {
				container.items[itemId] = quantity.get<int>();
			}
		}
		else//No items found, making the container redundant
			std::cout << "WARNING: Container (" << id << ") contains no items. Fix the JSON, plz.\n";
		if(definition.contains("keys") && definition["keys"].is_array()) {
			for (const auto& keyId : definition["keys"]) {
				container.keys.push_back(keyId.get<std::string>());
			}
		}
		_containers[id] = container;
	}
	std::cout << "Loaded " << _containers.size() << " Containers from " << filename << std::endl;
	return true;
}

bool ActorDatabase::loadDoorFromJson(const std::string& filename){
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open Door JSON file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing Door JSON file: " << e.what() << std::endl;
		return false;
	}

	_doors.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		DoorDefinition door;
		door.id = id;
		door.name = definition.value("name", "");
		door.description = definition.value("description", "");
		if (definition.contains("keys") && definition["keys"].is_array()) {//Should have keys because open doors are redundant
			for (const auto& keyId : definition["keys"]) {
				door.keys.push_back(keyId.get<std::string>());
			}
		}
		else//Door is open by default, making it redundant
			std::cout << "WARNING: Door (" << id << ") requires no keys, thus making it redundant. Either remove door from JSON, or add keys, plz.\n";
		_doors[id] = door;
	}
	std::cout << "Loaded " << _doors.size() << " Doors from " << filename << std::endl;
	return true;
}

bool ActorDatabase::loadMerchantFromJson(const std::string& filename) {
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open Merchant JSON file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing Merchant JSON file: " << e.what() << std::endl;
		return false;
	}

	_merchants.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		MerchantDefinition merchant;
		merchant.id = id;
		merchant.name = definition.value("name", "");
		merchant.description = definition.value("description", "");
		if (definition.contains("stock") && definition["stock"].is_array()) {
			for (const auto& itemId : definition["stock"]) {
				merchant.stock.push_back(itemId.get<std::string>());
			}
		}
		else
			std::cout << "WARNING: Merchant (" << id << ") has no items to sell. Fix the JSON, plz.\n";
		_merchants[id] = merchant;
	}
	std::cout << "Loaded " << _merchants.size() << " Merchants from file: " << filename << std::endl;
	return true;
}

bool ActorDatabase::loadEnemyFromJson(const std::string& filename) {
	std::fstream file(filename);
	if (!file) {
		std::cerr << "Could not open Enemy JSON file: " << filename << std::endl;
		return false;
	}
	nlohmann::json jsonData;
	try {
		file >> jsonData;
	}
	catch (const std::exception& e) {
		std::cerr << "Error occured while parsing Enemy JSON file: " << e.what() << std::endl;
		return false;
	}
	_enemies.reserve(jsonData.size());
	for (const auto& [id, definition] : jsonData.items()) {
		EnemyDefinition enemy;
		enemy.id = id;
		enemy.name = definition.value("name", "");
		enemy.description = definition.value("description", "");
		enemy.baseExpYield = definition.value("expYield", 0);
		enemy.stats = parseActorStats(definition.value("stats",nlohmann::json()));
		if (definition.contains("loot") && definition["loot"].is_object()) {
			for (const auto& [itemId, quantity] : definition["loot"].items()) {
				enemy.lootTable[itemId] = quantity.get<int>();
			}
		}
		_enemies[id] = enemy;
	}

	std::cout << "Loaded " << _enemies.size() << " Enemies from file: " << filename << std::endl;
	return true;
}

const NPCDefinition* ActorDatabase::findNPC(const std::string& id) const {
	auto it = _npcs.find(id);
	return it != _npcs.end() ? &it->second : nullptr;
}

const ContainerDefinition* ActorDatabase::findContainer(const std::string& id) const {
	auto it = _containers.find(id);
	return it != _containers.end() ? &it->second : nullptr;
}

const DoorDefinition* ActorDatabase::findDoor(const std::string& id) const {
	auto it = _doors.find(id);
	return it != _doors.end() ? &it->second : nullptr;
}

const MerchantDefinition* ActorDatabase::findMerchant(const std::string& id) const {
	auto it = _merchants.find(id);
	return it != _merchants.end() ? &it->second : nullptr;
}

const EnemyDefinition* ActorDatabase::findEnemy(const std::string& id) const {
	auto it = _enemies.find(id);
	return it != _enemies.end() ? &it->second : nullptr;
}



