#include <array>
#include <unordered_map>
#include <string>

//Enum 1: Combat stats
enum class CombatStat : uint8_t{
	HP, MAXHP,
	MP, MAXMP,
	POWER,		//Physical damage
	FORTITUDE,	//Physical defense
	SORCERY,	//Magical damage
	WILLPOWER,	//Magical defense
	SPEED,		//Determines turn order
	LUCK,		//For random stuff. Undetermined at this point
	UNKNOWN,
	COUNT
	};

//Enum 2: Growth stats
enum class GrowthStat : uint8_t {
	VITALITY,		//HP growth
	WISDOM,			//MP growth
	STRENGTH,		//Physical damage growth
	ENDURANCE,		//Physical defense growth
	INTELLIGENCE,	//Magical damage growth
	RESOLVE,		//Magical defense growth
	AGILITY,		//Speed growth
	FATE,			//Luck growth
	COUNT
};

struct ActorStats {
	std::array<int, (size_t)GrowthStat::COUNT> baseStats{};
	std::array<float, (size_t)GrowthStat::COUNT> growthRates{};
};

struct StatModifiers {
	std::array<int, (size_t)CombatStat::COUNT> flatModifiers{};
	std::array<float, (size_t)CombatStat::COUNT> multipliers{};

	StatModifiers() {
		flatModifiers.fill(0);
		multipliers.fill(1.0f);
	}
};