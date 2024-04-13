#include "PokemonData.h"

#include <fstream>

#include <rapidjson/filereadstream.h>
#include "rapidjson/filewritestream.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace Pokemon
{
	void SaveSpecieData(const int nationalDexNumber, const sSpeciesData& data)
	{
		// Isn't a valid national dex number
		if (nationalDexNumber <= 0 || nationalDexNumber >= 1008) return;
	
		std::string dexNumberString = std::to_string(nationalDexNumber);
		while (dexNumberString.length() < 4)
		{
			dexNumberString = "0" + dexNumberString;
		}
	
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

		d.AddMember("isStatsGenderBased", data.isStatsGenderBased, allocator);
		d.AddMember("isSpriteGenderBased", data.isSpriteGenderBased, allocator);

		rapidjson::Value defaultForm(rapidjson::kObjectType);

		defaultForm.AddMember("baseHp", data.defaultForm.baseStats.hp, allocator);
		defaultForm.AddMember("baseAtk", data.defaultForm.baseStats.atk, allocator);
		defaultForm.AddMember("baseSpAtk", data.defaultForm.baseStats.spAtk, allocator);
		defaultForm.AddMember("baseDef", data.defaultForm.baseStats.def, allocator);
		defaultForm.AddMember("baseSpDef", data.defaultForm.baseStats.spDef, allocator);
		defaultForm.AddMember("baseSpd", data.defaultForm.baseStats.spd, allocator);

		defaultForm.AddMember("type1", data.defaultForm.type1, allocator);
		defaultForm.AddMember("type2", data.defaultForm.type2, allocator);

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
		if (nationalDexNumber == 0 || nationalDexNumber >= 1008)
		{
			// Isn't a valid national dex number
			data.nationalDexNumber = 0;
			return;
		}
	
		std::string dexNumberString = std::to_string(nationalDexNumber);
		while (dexNumberString.length() < 4)
		{
			dexNumberString = "0" + dexNumberString;
		}
	
		FILE* fp = 0;
		fopen_s(&fp, ("assets/pokemon/" + dexNumberString + "/" + dexNumberString + ".json").c_str(), "rb"); // non-Windows use "r"
		if(fp == 0)
		{
			// File doesn't exists
			data.nationalDexNumber = 0;
			return;
		}
	
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	
		rapidjson::Document d;
		d.ParseStream(is);
	
		fclose(fp);

		if (d["data_version"].GetInt() != JSON_DATA_VERSION)
		{
			std::cout << "WARNING: loader function has a different version as json data" << std::endl;
		}

		// TODO: actually load it
	}
}