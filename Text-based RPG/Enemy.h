#pragma once
#include "Actor.h"

class Enemy : public Actor {
private:
	ActorStats _stats;
	Inventory _loot;
	std::vector<float> _lootDropRates{};
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> _equipped{};

	bool equals(const Actor& other) const override {
		if (!Actor::equals(other)) return false;
		const auto& e = static_cast<const Enemy&>(other);
		return _stats.baseStats == e._stats.baseStats && _stats.growthRates == e._stats.growthRates && _loot == e._loot && _lootDropRates == e._lootDropRates && _equipped == e._equipped;
	}
public:
	Enemy(std::string id, const std::string& name, const std::string description, ActorStats stats, Inventory loot, std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> equipped) : Actor(id, name, description), _stats(stats), _loot(loot), _equipped(equipped) {}
	ActorStats& GetStats() { return _stats; }
	const ActorStats& GetStats() const { return _stats; }
	Inventory& GetLoot() { return _loot; }
	const Inventory& GetLoot() const { return _loot; }
	std::array<std::shared_ptr<Equipment>, (size_t)EquipmentSlot::COUNT> GetEquipped() const { return _equipped; }

	std::unique_ptr<Actor> clone() const override {
		return std::make_unique<Enemy>(*this);
	}

	void physicalAttack(Player& player);
	void magicAttack(Player& player);
};