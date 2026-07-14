#pragma once
#include <string>
#include <unordered_map>
#include "PlayerClassDefinition.h"

class PlayerClassDatabase {
private:
	std::unordered_map<std::string, PlayerClassDefinition> _classes;
public:
	bool loadFromJson(const std::string& filename);
	const PlayerClassDefinition* find(const std::string& id) const;
};