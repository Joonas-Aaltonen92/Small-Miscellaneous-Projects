#pragma once
#include <string>
#include <unordered_map>
#include "Stats.h"

enum class ItemType {
	EQUIPMENT,
	CONSUMABLE,//Food and potions and such
	TRINKET, //Just for selling
	KEYITEM,//Important things like quest items
	KEY,//Normal keys/items that open doors and chests
	UNKNOWN,//Undefined. Basically for errors and such
	COUNT
};

enum class EquipmentSlot {
	HEAD,
	CHEST,
	WAIST,//Belts
	LEGS,
	FEET,
	ARMS,
	HAND,
	RING,
	AMULET,
	COUNT
};

struct ItemDefinition {
	std::string id;
	std::string name;
	std::string description;

	ItemType type = ItemType::UNKNOWN;

	int value = 0;

	std::unordered_map<EquipmentSlot, int> slotCosts;
	
	StatModifiers modifiers;

	bool stackable = false;
	int maxStackSize = 1;

	[[nodiscard]] bool isEquipment() const { return type == ItemType::EQUIPMENT; }
	[[nodiscard]] bool isConsumable() const { return type == ItemType::CONSUMABLE; }
	[[nodiscard]] bool isTrinket() const { return type == ItemType::TRINKET; }
	[[nodiscard]] bool isKeyItem() const { return type == ItemType::KEYITEM; }
	[[nodiscard]] bool isKey() const { return type == ItemType::KEY; }
	[[nodiscard]] bool isEquippable() const { return !slotCosts.empty(); }
};