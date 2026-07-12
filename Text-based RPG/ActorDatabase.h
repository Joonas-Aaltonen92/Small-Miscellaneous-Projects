#pragma once
#include <unordered_map>
#include <string>
#include "ActorDefinitions.h"

class ActorDatabase {
private:
	std::unordered_map<std::string, NPCDefinition> _npcs;
	std::unordered_map<std::string, ContainerDefinition> _containers;
	std::unordered_map<std::string, DoorDefinition> _doors;
	std::unordered_map<std::string, MerchantDefinition> _merchants;
	std::unordered_map<std::string, EnemyDefinition> _enemies;
public:
	bool loadNPCFromJson(const std::string& fileName);
	bool loadContainerFromJson(const std::string& fileName);
	bool loadDoorFromJson(const std::string& fileName);
	bool loadMerchantFromJson(const std::string& fileName);
	bool loadEnemyFromJson(const std::string& fileName);
	void loadActors() {
		loadNPCFromJson("npcs.json");
		loadContainerFromJson("containers.json");
		loadDoorFromJson("doors.json");
		loadMerchantFromJson("merchants.json");
		loadEnemyFromJson("enemies.json");
	}

	const NPCDefinition* findNPC(const std::string& id) const;
	const ContainerDefinition* findContainer(const std::string& id) const;
	const DoorDefinition* findDoor(const std::string& id) const;
	const MerchantDefinition* findMerchant(const std::string& id) const;
	const EnemyDefinition* findEnemy(const std::string& id) const;
};