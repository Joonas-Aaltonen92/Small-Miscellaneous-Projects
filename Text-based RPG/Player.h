#pragma once
#include <string>
#include <memory>
#include <array>
#include "Stats.h"
#include "Inventory.h"

class Item;



class Player{
private:
	std::string _name;
	int _level = 1;
	int _experience = 0;
	ActorStats _stats;
	Inventory _inventory;
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};
	int _money = 0;
	int _walletSize = 999;//Small wallet
public:
	Player(std::string name, Inventory inventory, ActorStats stats, std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> equipped) : _name(name), _stats(stats), _inventory(inventory), _equipped(equipped) {}

	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetInventory() { return _inventory; }
	const Inventory& GetInventory() const { return _inventory; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

	//Json things
	void loadPlayer();
	void savePlayer();

	//Player actions
	void talk(NPC npc);
	void physicalAttack(Enemy& enemy);
	void magicAttack(Enemy& enemy);
	void openContainer(Container& container) {
		container.open();//Prints the insides.
		const auto& stacks = container.GetInventory().getStacks();
		for (const auto& stack : stacks)
			addItemToInventory(stack.first, stack.second);
	}
	void addItemToInventory(std::string itemID, int quantity = 1) {
		_inventory.addItem(itemID, quantity);
	}
	void removeItemFromInventory(const std::string& itemID, int quantity = 1) {
		_inventory.removeItem(itemID, quantity);
	}
	//Simple print functions. The actual function will be handled by Game.cpp
	void lookAround() {
		std::println("You take a look around...");
	}
	void search() {
		std::println("You rummage around...");
	}

	void addMoney(int amount) {
		_money += amount;
		if (_money > _walletSize)
			_money = _walletSize;
	}
	void removeMoney(int amount) {
		_money -= amount;
		if(_money < 0)
			_money = 0;
	}
	void increaseWalletSize(int newSize) {
		if (newSize < _walletSize) //In case we accidentally give the player a smaller wallet
			_walletSize += newSize;
		else
			_walletSize = newSize;
	}
};