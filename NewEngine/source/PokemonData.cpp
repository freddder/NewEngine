#include "PokemonData.h"

#include <fstream>

#include <rapidjson/filereadstream.h>
#include "rapidjson/filewritestream.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace Pokemon
{
	bool OpenPokemonDataFile(rapidjson::Document& doc, const int nationalDexNumber);
	void LoadFormData(rapidjson::Value& formObject, sForm& form);

	std::string MakeDexNumberFolderName(const int nationalDexNumber)
	{
		std::string dexNumberString = std::to_string(nationalDexNumber);
		while (dexNumberString.length() < 4)
		{
			dexNumberString = "0" + dexNumberString;
		}

		return dexNumberString;
	}

	void SaveSpecieData(const int nationalDexNumber, const sSpeciesData& data)
	{
		if (!IsNationalDexNumberValid(nationalDexNumber)) return;
	
		std::string dexNumberString = MakeDexNumberFolderName(nationalDexNumber);
	
		FILE* fp = 0;
		fopen_s(&fp, ("assets/pokemon/" + dexNumberString + "/" + dexNumberString + ".json").c_str(), "wb");
		if (fp == 0)
		{
			std::cout << "Failed to create file" << std::endl;
			return;
		}
	
		char writeBuffer[65536];
		rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	
		rapidjson::Document d;
		rapidjson::Document::AllocatorType& allocator = d.GetAllocator();
	
		d.SetObject();

		d.AddMember("data_version", JSON_DATA_VERSION, allocator);
		rapidjson::Value name(data.name.c_str(), allocator);
		d.AddMember("name", name, allocator);
		d.AddMember("nationalDexNumber", data.nationalDexNumber, allocator);

		d.AddMember("genderRatio", data.genderRatio, allocator);
		d.AddMember("eggGroup1", data.eggGroup1, allocator);
		d.AddMember("eggGroup2", data.eggGroup2, allocator);
		d.AddMember("hatchCycles", data.hatchCycles, allocator);

		d.AddMember("catchRate", data.catchRate, allocator);

		d.AddMember("isStatsGenderBased", data.isFormGenderBased, allocator);
		d.AddMember("isSpriteGenderBased", data.isSpriteGenderBased, allocator);

		rapidjson::Value defaultForm(rapidjson::kObjectType);

		defaultForm.AddMember("type1", data.defaultForm.type1, allocator);
		defaultForm.AddMember("type2", data.defaultForm.type2, allocator);

		defaultForm.AddMember("baseHp", data.defaultForm.baseStats.hp, allocator);
		defaultForm.AddMember("baseAtk", data.defaultForm.baseStats.atk, allocator);
		defaultForm.AddMember("baseSpAtk", data.defaultForm.baseStats.spAtk, allocator);
		defaultForm.AddMember("baseDef", data.defaultForm.baseStats.def, allocator);
		defaultForm.AddMember("baseSpDef", data.defaultForm.baseStats.spDef, allocator);
		defaultForm.AddMember("baseSpd", data.defaultForm.baseStats.spd, allocator);

		defaultForm.AddMember("height", data.defaultForm.height, allocator);
		defaultForm.AddMember("weight", data.defaultForm.weight, allocator);

		d.AddMember("defaultForm", defaultForm, allocator);

		rapidjson::Value alternateForms(rapidjson::kArrayType);
		for (std::map<std::string, sForm>::const_iterator it = data.alternateForms.begin(); it != data.alternateForms.end(); it++)
		{
			rapidjson::Value newAlternateForm(rapidjson::kObjectType);
			rapidjson::Value alternateFormId(it->first.c_str(), allocator);
			newAlternateForm.AddMember("formId", alternateFormId, allocator);

			newAlternateForm.AddMember("baseHp", it->second.baseStats.hp, allocator);
			newAlternateForm.AddMember("baseAtk", it->second.baseStats.atk, allocator);
			newAlternateForm.AddMember("baseSpAtk", it->second.baseStats.spAtk, allocator);
			newAlternateForm.AddMember("baseDef", it->second.baseStats.def, allocator);
			newAlternateForm.AddMember("baseSpDef", it->second.baseStats.spDef, allocator);
			newAlternateForm.AddMember("baseSpd", it->second.baseStats.spd, allocator);

			newAlternateForm.AddMember("type1", it->second.type1, allocator);
			newAlternateForm.AddMember("type2", it->second.type2, allocator);

			newAlternateForm.AddMember("height", it->second.height, allocator);
			newAlternateForm.AddMember("weight", it->second.weight, allocator);

			alternateForms.PushBack(newAlternateForm, allocator);
		}
		d.AddMember("alternateForms", alternateForms, allocator);
	
		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		d.Accept(writer);
	
		fclose(fp);	
	}
	
	void LoadSpecieData(const int nationalDexNumber, sSpeciesData& data)
	{
		rapidjson::Document d;
		if (!OpenPokemonDataFile(d, nationalDexNumber))
		{
			data.nationalDexNumber = 0;
			return;
		}

		if (d["data_version"].GetInt() != JSON_DATA_VERSION)
		{
			std::cout << "WARNING: loader function has a different version as json data" << std::endl;
		}

		data.name = d["name"].GetString();
		data.nationalDexNumber = d["nationalDexNumber"].GetInt();
		data.genderRatio = d["genderRatio"].GetInt();
		data.eggGroup1 = static_cast<eEggGroup>(d["eggGroup1"].GetInt());
		data.eggGroup2 = static_cast<eEggGroup>(d["eggGroup2"].GetInt());
		data.hatchCycles = d["hatchCycles"].GetInt();
		data.catchRate = d["catchRate"].GetFloat();
		data.isFormGenderBased = d["isStatsGenderBased"].GetBool();
		data.isSpriteGenderBased = d["isSpriteGenderBased"].GetBool();

		rapidjson::Value& defaultFormObject = d["defaultForm"];
		LoadFormData(defaultFormObject, data.defaultForm);

		rapidjson::Value& alternateForms = d["alternateForms"];
		for (unsigned int i = 0; i < alternateForms.Size(); i++)
		{
			rapidjson::Value& newFormObject = alternateForms[i];
			sForm newForm;
			LoadFormData(newFormObject, newForm);

			std::string formName = newFormObject["name"].GetString();
			data.alternateForms.insert(std::pair<std::string, sForm>(formName, newForm));
		}
	}

	sRoamingPokemonData GenerateRoamingPokemonData(const sSpawnData& spawnData)
	{
		sRoamingPokemonData newRoamingData;

		newRoamingData.nationalDexNumber = spawnData.nationalDexNumber;
		newRoamingData.formName = spawnData.formName;
		newRoamingData.isFormGenderBased = spawnData.isFormGenderBased;
		newRoamingData.isSpriteGenderBased = spawnData.isSpriteGenderBased;

		// Determine level
		newRoamingData.level = rand() % (spawnData.maxLevel - spawnData.minLevel + 1) + spawnData.minLevel;

		// Determine gender
		newRoamingData.gender = Pokemon::NO_GENDER;
		if (spawnData.genderRatio >= 0) // not genderless
		{
			int genderRandom = (rand() % 100); // [0-99]

			if (spawnData.genderRatio < genderRandom) newRoamingData.gender = Pokemon::MALE;
			else newRoamingData.gender = Pokemon::FEMALE;
		}

		// Determine shiny
		int shinyRandom = (rand() % 100); // [0-99]
		if (shinyRandom < 50) newRoamingData.isShiny = true;

		return newRoamingData;
	}

	// TODO: have a more versitile way to create battle ready pokemon & incorporate into entering wild encounters and spawn data
	sIndividualData GenerateIndividualPokemonData(int nationalDexId)
	{
		//if (!IsNationalDexNumberValid(nationalDexId)) return;

		return sIndividualData();
	}

	sBattleData GeneratePokemonBattleData(const sRoamingPokemonData& roamingData)
	{
		sBattleData newData;
		newData.nationalDexNumber = roamingData.nationalDexNumber;
		newData.formName = roamingData.formName;
		newData.gender = roamingData.gender;
		newData.level = roamingData.level;
		newData.isShiny = roamingData.isShiny;
		newData.isFormGenderBased = roamingData.isFormGenderBased;
		newData.isSpriteGenderBased = roamingData.isSpriteGenderBased;

		rapidjson::Document d;
		OpenPokemonDataFile(d, roamingData.nationalDexNumber);

		if (newData.isFormGenderBased && newData.gender == FEMALE) // load "female" form
		{
			rapidjson::Value& alternateForms = d["alternateForms"];
			for (unsigned int i = 0; i < alternateForms.Size(); i++)
			{
				if (alternateForms[i]["name"].GetString() == "female")
				{
					LoadFormData(alternateForms[i], newData.form);
					break;
				}
			}
		}
		else if (newData.formName != "") // load custom form
		{
			rapidjson::Value& alternateForms = d["alternateForms"];
			for (unsigned int i = 0; i < alternateForms.Size(); i++)
			{
				if (alternateForms[i]["name"].GetString() == newData.formName)
				{
					LoadFormData(alternateForms[i], newData.form);
					break;
				}
			}
		}
		else // load default form
		{
			rapidjson::Value& defaultFormObject = d["defaultForm"];
			LoadFormData(defaultFormObject, newData.form);
		}

		newData.name = d["name"].GetString();

		newData.maxHealth = 100;
		newData.currHealth = 100;

		newData.expToNextLevel = 2000;
		newData.currExp = 0;

		return newData;
	}

	const std::string sRoamingPokemonData::MakeRoamingTextureName()
	{
		std::string textureName = std::to_string(nationalDexNumber);

		if (gender == FEMALE && (isSpriteGenderBased || isFormGenderBased))
			textureName = textureName + "_f";
		else if (formName != "") // There is no case where both will be true
			textureName = textureName + "_" + formName;

		if (isShiny)
			textureName = textureName + "_s";

		textureName = textureName + ".png";

		return textureName;
	}

	const std::string sIndividualData::MakeIconTextureName()
	{
		std::string textureName = std::to_string(nationalDexNumber);

		if (gender == FEMALE && isFormGenderBased)
			textureName = textureName + "_f";
		else if (formName != "") // There is no case where both will be true
			textureName = textureName + "_" + formName;

		textureName += "_ico";

		if (isShiny)
			textureName = textureName + "_s";

		textureName = textureName + ".png";

		return textureName;
	}

	const std::string sIndividualData::MakeBattleTextureName(bool isFront)
	{
		std::string textureName = std::to_string(nationalDexNumber);

		if (gender == FEMALE && (isSpriteGenderBased || isFormGenderBased))
			textureName = textureName + "_f";
		else if (formName != "") // There is no case where both will be true
			textureName = textureName + "_" + formName;

		if (isFront)
			textureName = textureName + "_bf";
		else
			textureName = textureName + "_bb";

		if (isShiny)
			textureName = textureName + "_s";

		textureName = textureName + ".png";

		return textureName;
	}

	void sIndividualData::LoadFormFromSpeciesData()
	{
		if (!IsNationalDexNumberValid(nationalDexNumber)) return;

		sSpeciesData speciesData;
		LoadSpecieData(nationalDexNumber, speciesData);

		if (speciesData.isFormGenderBased && gender == FEMALE)
			form = speciesData.alternateForms["female"];
		else if (formName != "")
			form = speciesData.alternateForms[formName];
		else
			form = speciesData.defaultForm;

		if (name == "")
			name = speciesData.name;
	}

	bool OpenPokemonDataFile(rapidjson::Document& doc, const int nationalDexNumber)
	{
		if (!IsNationalDexNumberValid(nationalDexNumber)) return false;

		std::string dexNumberString = MakeDexNumberFolderName(nationalDexNumber);

		FILE* fp = 0;
		fopen_s(&fp, ("assets/pokemon/" + dexNumberString + "/" + dexNumberString + ".json").c_str(), "rb"); // non-Windows use "r"
		if (fp == 0) return false; // File doesn't exists

		// OPTIMIZATION: best buffer size might be different
		char readBuffer[4096];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

		doc.ParseStream(is);

		fclose(fp);
		return true;
	}

	void LoadFormData(rapidjson::Value& formObject, sForm& form)
	{
		form.type1 = static_cast<eType>(formObject["type1"].GetInt());
		form.type2 = static_cast<eType>(formObject["type2"].GetInt());

		form.baseStats.hp = formObject["baseHp"].GetInt();
		form.baseStats.atk = formObject["baseAtk"].GetInt();
		form.baseStats.spAtk = formObject["baseSpAtk"].GetInt();
		form.baseStats.def = formObject["baseDef"].GetInt();
		form.baseStats.spDef = formObject["baseSpDef"].GetInt();
		form.baseStats.spd = formObject["baseSpd"].GetInt();

		form.height = formObject["height"].GetFloat();
		form.weight = formObject["weight"].GetFloat();

		form.battleSpriteHeightSize = formObject["battleSpriteHeightSize"].GetFloat();
		form.battleFrontSpriteFrameCount = formObject["battleFrontSpriteFrameCount"].GetInt();
		form.battleBackSpriteFrameCount = formObject["battleBackSpriteFrameCount"].GetInt();
	}
}