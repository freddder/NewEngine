#pragma once
#include <string>
#include <vector>

namespace Pokemon
{
	enum Type
	{
		NORMAL,
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
		NO_TYPE
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
		EGG_NO_EGGS_DISCOVERED
	};

	struct Form
	{
		std::string formName = ""; // Display name for Pokedex (same as species if not set) (might remove later)
		std::string formId = "default"; // Name for looking up sprite file name

		int baseHp;
		int baseAtk;
		int baseSpAtk;
		int baseDef;
		int baseSpDef;
		int baseSpd;
		
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
		int nationalDexNumber = -1;

		bool isGenderless = false;
		float genderRatio = 0.5f; // chance to be male (0 to 1)
		EggGroup eggGroup = EGG_NO_EGGS_DISCOVERED;
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
}