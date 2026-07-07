#pragma once
#include <unordered_map>
#include <string>
#include "ActorDefinitions.h"

class ActorDatabase {
private:
	std::unordered_map<std::string, NPCDefinition> _NPCs;
	std::unordered_map<std::string, ContainerDefinition> _containers;
	std::unordered_map<std::string, DoorDefinition> _doors;
	std::unordered_map<std::string, MerchantDefinition> _merchants;
	std::unordered_map<std::string, EnemyDefinition> _enemies;
public:
	bool loadFromJson(const std::string& fileName);

	const NPCDefinition* findNPC(const std::string& id) const;
	const ContainerDefinition* findContainer(const std::string& id) const;
	const DoorDefinition* findDoor(const std::string& id) const;
	const MerchantDefinition* findMerchant(const std::string& id) const;
	const EnemyDefinition* findEnemy(const std::string& id) const;
};