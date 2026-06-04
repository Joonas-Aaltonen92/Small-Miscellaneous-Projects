#pragma once
#include <string>
#include <memory>
#include <array>
#include "Stats.h"
#include "Inventory.h"


class Player{
private:
	std::string _name;
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

	void talk(NPC npc) {
		int counter = 0;
		while (counter < npc.GetDialogue().size()) {
			std::println("{}: {}", npc.GetName(), npc.GetDialogue()[counter]);
			counter++;

			std::cin.get();
		}
	}
	void physicalAttack(Enemy& enemy) {
		int playerPower = _stats.baseStats[(size_t)CombatStat::POWER]; //Player power without equipment or buffs.
		//Loop through equipped items and add modifiers.
		for (auto& e : _equipped) {
			if (e) {
				playerPower += e->getModifiers().flatModifiers[(size_t)CombatStat::POWER];
				playerPower = static_cast<int>(playerPower * e->getModifiers().multipliers[(size_t)CombatStat::POWER]);//This works only if multiplier isn't 0, otherwise things go to shit.
			}
		}
		int enemyDefense = enemy.GetStats().baseStats[(size_t)CombatStat::FORTITUDE];
		for (auto& e : enemy.GetEquipped()) {
			enemyDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::FORTITUDE];
			enemyDefense = static_cast<int>(enemyDefense * e->getModifiers().multipliers[(size_t)CombatStat::FORTITUDE]);//Again, only works if multiplier is not 0.
		}

		//Calculate damage based on some formula, IDK
		int damage = enemyDefense - playerPower;
		if (damage < 0)
			damage = 0;//Make sure we're not healing the enemy.
		enemy.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("You attacked, doing {} points of damage!", damage);
	}

	void magicAttack(Enemy& enemy) {
		int playerPower = _stats.baseStats[(size_t)CombatStat::SORCERY];
		for (auto& e : _equipped) {
			if (e) {
				playerPower += e->getModifiers().flatModifiers[(size_t)CombatStat::SORCERY];
				playerPower = static_cast<int>(playerPower * e->getModifiers().multipliers[(size_t)CombatStat::SORCERY]);
			}
		}
		int enemyDefense = enemy.GetStats().baseStats[(size_t)CombatStat::WILLPOWER];
		for (auto& e : _equipped) {
			if (e) {
				enemyDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::WILLPOWER];
				enemyDefense = static_cast<int>(enemyDefense * e->getModifiers().multipliers[(size_t)CombatStat::WILLPOWER]);
			}
		}

		int damage = playerPower - enemyDefense;
		if (damage < 0)
			damage = 0;
		enemy.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
		std::println("You threw a fireball, doing {} points of damage!", damage);
	}
};