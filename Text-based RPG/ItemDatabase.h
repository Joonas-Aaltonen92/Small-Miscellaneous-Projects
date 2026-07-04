#pragma once
#include <unordered_map>
#include <string>
#include "ItemDefinition.h"

class ItemDatabase {
private:
	std::unordered_map<std::string, ItemDefinition> _items;
public:
	bool loadFromJson(const std::string& filename);
	const ItemDefinition* find(const std::string& id) const;	
};