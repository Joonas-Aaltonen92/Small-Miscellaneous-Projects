#include "Player.h"




//Player actions
void Player::talk(NPC npc) {
	int counter = 0;
	while (counter < npc.GetDialogue().size()) {
		std::println("{}: {}", npc.GetName(), npc.GetDialogue()[counter]);
		counter++;
		std::cin.get();
	}
}

void Player::physicalAttack(Enemy& enemy) {
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

void Player::magicAttack(Enemy& enemy) {
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

void Player::openContainer(Container& container) {
	container.open();
	
}