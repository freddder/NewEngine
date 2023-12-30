#pragma once
#include <string>

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
		AMORPHOUS,
		BUG,
		DITTO,
		DRAGON,
		FAIRY,
		FIELD,
		FLYING,
		GRASS,
		MINERAL,
		MONSTER,
		HUMAN_LIKE,
		WATER_1,
		WATER_2,
		WATER_3,
		NO_EGGS_DISCOVERED
	};

	struct SpeciesData
	{
		std::string name;
		int nationalDexNumber;

		bool genderless = false;
		float genderRatio = 0.5f; // chance to be male (0 to 1)
		EggGroup eggGroup = NO_EGGS_DISCOVERED;
		int hatchCycles; // Not sure what this means, but its related to the amount of steps to hatch

		float catchRatio; // Not sure how this will work either
	};
}