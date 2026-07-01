#include <cstddef>
#include <cstdint>
#include <array>

inline constexpr size_t COMBAT_STAT_COUNT = 11;
inline constexpr size_t GROWTH_STAT_COUNT = 9;

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
	UNKNOWN,
	COUNT
};

struct ActorStats {
	std::array<int, COMBAT_STAT_COUNT> baseStats{};
	std::array<float, GROWTH_STAT_COUNT> growthRates{};
};

struct StatModifiers {
	std::array<int, COMBAT_STAT_COUNT> flatModifiers{};
	std::array<float, COMBAT_STAT_COUNT> multipliers{};

	StatModifiers() {
		flatModifiers.fill(0);
		multipliers.fill(1.0f);
	}
};