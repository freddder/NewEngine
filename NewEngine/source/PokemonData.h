#pragma once
#include <string>
#include <vector>

namespace Pokemon
{
	enum Type
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

	enum EggGroup
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

	enum Gender
	{
		MALE,
		FEMALE,
		NO_GENDER
	};

	struct Stats
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

	struct Form
	{
		//std::string formName = ""; // Display name for Pokedex (same as species if not set) (might remove later)
		std::string formId = "default"; // Name for looking up sprite file name

		Stats baseStats;
		
		// Ability ability1
		// Ability ability2
		// Ability hiddenAbility

		Type type1;
		Type type2 = NO_TYPE;

		float height; // in meters
		float weight; // in kilograms

		// Learnset as a pair of int (level) and int (move id)
	};

	struct SpeciesData
	{
		std::string name;
		int nationalDexNumber = 0;

		bool isGenderless = false;
		float genderRatio = 0.5f; // chance to be male (0 to 1)
		EggGroup eggGroup1 = EGG_NO_EGGS_DISCOVERED;
		EggGroup eggGroup2 = EGG_NO_EGG_GROUP;
		int hatchCycles; // Not sure what this means, but its related to the amount of steps to hatch

		float catchRate; // Not sure how this will work either

		bool isStatsGenderBased = false; // Use an alternate form if female. Will only be used a few times (ex: Meowstic, Indeedee)
		bool isSpriteGenderBased = false; // Change sprite if its female (doesn't matter if isStatsGenderBased is true)

		Form defaultForm;
		std::vector<Form> alternateForms;
	};

	const static unsigned int JSON_DATA_VERSION = 1;
	void SaveSpecieData(const int nationalDexNumber, const SpeciesData& data);
	void LoadSpecieData(const int nationalDexNumber, SpeciesData& data);

	struct OverworldPokemonData
	{
		int nationalDexNumber = 0;
		std::string formName = "";

		Gender gender;
		bool shiny = false;

		// Ability ability;
		// Natire nature;
		Stats IVs; // Individial values
		Stats EVs; // Effort values

		// HeldItem item;

		// Move move1;
		// Move move2;
		// Move move3;
		// Move move4;
	};
}