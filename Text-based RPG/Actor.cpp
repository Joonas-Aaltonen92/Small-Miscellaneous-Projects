#include "Actor.h"
#include "Player.h"

void Enemy::physicalAttack(Player& player) {
	int enemyPower = _stats.baseStats[(size_t)CombatStat::POWER];
	float multiplier = 1.0f;
	for (auto& e : _equipped) {
		if (e) {
			enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::POWER];
			multiplier += e->getModifiers().multipliers[(size_t)CombatStat::POWER];
		}
	}
	enemyPower = static_cast<int>(enemyPower * multiplier);

	int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::FORTITUDE];
	float defMultiplier = 1.0f;
	for (auto& e : _equipped) {
		if (e) {
			playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::FORTITUDE];
			defMultiplier += e->getModifiers().multipliers[(size_t)CombatStat::FORTITUDE];
		}
	}
	playerDefense = static_cast<int>(playerDefense * defMultiplier);

	int damage = playerDefense - enemyPower;
	if (damage < 0)
		damage = 0;
	player.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
	std::println("{} attacked, doing {} points of damage!", _name, damage);
}

void Enemy::magicAttack(Player& player) {
	int enemyPower = _stats.baseStats[(size_t)CombatStat::SORCERY];
	float multiplier = 1.0f;
	for (auto& e : _equipped) {
		if (e) {
			enemyPower += e->getModifiers().flatModifiers[(size_t)CombatStat::SORCERY];
			multiplier += e->getModifiers().multipliers[(size_t)CombatStat::SORCERY];
		}
	}
	enemyPower = static_cast<int>(enemyPower * multiplier);
	int playerDefense = player.GetStats().baseStats[(size_t)CombatStat::WILLPOWER];
	float defMultiplier = 1.0f;
	for (auto& e : _equipped) {
		if (e) {
			playerDefense += e->getModifiers().flatModifiers[(size_t)CombatStat::WILLPOWER];
			defMultiplier += e->getModifiers().multipliers[(size_t)CombatStat::WILLPOWER];
		}
	}
	playerDefense = static_cast<int>(playerDefense * defMultiplier);

	int damage = playerDefense - enemyPower;
	if (damage < 0)
		damage = 0;
	player.GetStats().baseStats[(size_t)CombatStat::HP] -= damage;
	std::println("{} sorcered, doing {} points of damage!", _name, damage);
}