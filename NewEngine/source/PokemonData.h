#pragma once
#include <string>
#include <map>

namespace Pokemon
{
	const static float BASE_SHINY_ODDS = 1.f / 8192.f;

	enum eType
	{
		NORMAL = 0,
		FIRE,
		WATER,
		ELECTRIC,
		GRASS,
		ICE,
		FIGHTING,
		POISON,
		GROUND,
		FLYING,
		PSYCHIC,
		BUG,
		ROCK,
		GHOST,
		DRAGON,
		STEEL,
		DARK,
		FAIRY,
		NO_TYPE,
		TYPE_ENUM_COUNT
	};

	const static char* Type_Strings[] =
	{
		"Normal",
		"Fire",
		"Water",
		"Electric",
		"Grass",
		"Ice",
		"Fighting",
		"Poison",
		"Ground",
		"Flying",
		"Psychic",
		"Bug",
		"Rock",
		"Ghost",
		"Dragon",
		"Steel",
		"Dark",
		"Fairy",
		"No Type"
	};

	enum eEggGroup
	{
		EGG_AMORPHOUS,
		EGG_BUG,
		EGG_DITTO,
		EGG_DRAGON,
		EGG_FAIRY,
		EGG_FIELD,
		EGG_FLYING,
		EGG_GRASS,
		EGG_MINERAL,
		EGG_MONSTER,
		EGG_HUMAN_LIKE,
		EGG_WATER_1,
		EGG_WATER_2,
		EGG_WATER_3,
		EGG_NO_EGGS_DISCOVERED,
		EGG_NO_EGG_GROUP,
		EGG_ENUM_COUNT
	};

	const static char* EggGroup_Strings[] =
	{
		"Amorphous",
		"Bug",
		"Ditto",
		"Dragon",
		"Fairy",
		"Field",
		"Flying",
		"Grass",
		"Mineral",
		"Monster",
		"Human Like",
		"Water 1",
		"Water 2",
		"Water 3",
		"No Eggs Discovered",
		"No Egg Group"
	};

	enum eGender
	{
		MALE,
		FEMALE,
		NO_GENDER
	};

	struct sStats
	{
		int hp = 0;		// Health
		int atk = 0;	// Attack
		int spAtk = 0;	// Special Attack
		int def = 0;	// Defence
		int spDef = 0;	// Special Defence
		int spd = 0;	// Speed

		int Total()
		{
			return hp + atk + spAtk + def + spDef + spd;
		}
	};

	static bool IsNationalDexNumberValid(int nationalDexNumber)
	{
		return nationalDexNumber > 0 && nationalDexNumber <= 1008;
	}

	struct sForm
	{
		sStats baseStats;
		
		// Ability ability1
		// Ability ability2
		// Ability hiddenAbility

		eType type1 = NORMAL;
		eType type2 = NO_TYPE;

		float height = 0.f; // in meters
		float weight = 0.f; // in kilograms

		float battleSpriteHeightSize = 1.f;
		int battleSpriteFrameCount = 1;

		// Learnset as a pair of int (level) and int (move id)
	};

	struct sSpeciesData
	{
		std::string name;
		int nationalDexNumber = 0;

		int genderRatio = 50; // chance to be male (0 to 100 and <0 for genderless)
		eEggGroup eggGroup1 = EGG_NO_EGGS_DISCOVERED;
		eEggGroup eggGroup2 = EGG_NO_EGG_GROUP;
		int hatchCycles = 0; // Not sure what this means, but its related to the amount of steps to hatch

		float catchRate; // Not sure how this will work either

		bool isFormGenderBased = false; // Use an alternate form if female. Will only be used a few times (ex: Meowstic, Indeedee)
		bool isSpriteGenderBased = false; // Change sprite if its female (doesn't matter if isStatsGenderBased is true)

		sForm defaultForm;
		std::map<std::string, sForm> alternateForms;
	};

	const static unsigned int JSON_DATA_VERSION = 1;
	void SaveSpecieData(const int nationalDexNumber, const sSpeciesData& data);
	void LoadSpecieData(const int nationalDexNumber, sSpeciesData& data);

	enum eSpawnType
	{
		TALL_GRASS,
		ST_WATER,
		CAVE,
		ST_ENUM_COUNT
	};

	struct sSpawnData
	{
		int nationalDexNumber = 0;
		std::string formName = "";

		// These should be the same as the species data
		int genderRatio = 50;
		bool isFormGenderBased = false; // Use an alternate form if female. Will only be used a few times (ex: Meowstic, Indeedee)
		bool isSpriteGenderBased = false; // Change sprite if its female (doesn't matter if isStatsGenderBased is true)

		eSpawnType spawnType = TALL_GRASS;
		int spawnChance = 0; // Number of "entries"

		int minLevel;
		int maxLevel;
	};

	struct sRoamingPokemonData // Minimal data for rendering sprite in overworld
	{
		int nationalDexNumber = 0;
		std::string formName = "";

		int level = 0;
		eGender gender = Pokemon::NO_GENDER;
		bool isShiny = false;

		bool isFormGenderBased = false;
		bool isSpriteGenderBased = false;

		const std::string MakeRoamingTextureName();
	};
	sRoamingPokemonData GenerateRoamingPokemonData(const sSpawnData& spawnData);

	struct sIndividualData : public sRoamingPokemonData // Individual data (outside of battle)
	{
		std::string customName = "";

		int maxHealth;
		int currHealth;

		int expToNextLevel;
		int currExp;

		sForm form;

		// Ability ability;
		// Natire nature;
		sStats IVs; // Individial values
		sStats EVs; // Effort values

		// eStatus condition
		// HeldItem item;

		// Move move1;
		// Move move2;
		// Move move3;
		// Move move4;

		const std::string MakeIconTextureName();
		const std::string MakeBattleTextureName(bool isFront = true);
	};
	sIndividualData GenerateIndividualPokemonData(int nationalDexId);

	struct sBattleData : public sIndividualData // Individual data (in battle)
	{
		sStats statChanges;
	};
	sBattleData GeneratePokemonBattleData(const sRoamingPokemonData& roamingData); // For wild encounters
}