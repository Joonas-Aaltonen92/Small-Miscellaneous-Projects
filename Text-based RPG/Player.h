#pragma once
#include <string>
#include <memory>
#include <array>
#include "Stats.h"
#include "Inventory.h"
#include "Item.h"
#include "Actor.h"


class Player{
private:
	std::string _name;
	int _level = 1;
	int _experience = 0;
	ActorStats _stats;
	Inventory _inventory;
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};
public:
	Player(std::string name, Inventory inventory, ActorStats stats, std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> equipped) : _name(name), _stats(stats), _inventory(inventory), _equipped(equipped) {}

	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetInventory() { return _inventory; }
	const Inventory& GetInventory() const { return _inventory; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

	void loadPlayer() {

	}


	void talk(NPC npc);
	void physicalAttack(Enemy& enemy);
	void magicAttack(Enemy& enemy);
};