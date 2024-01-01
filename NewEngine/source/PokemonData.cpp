#include "PokemonData.h"

#include <fstream>

#include <rapidjson/filereadstream.h>
#include "rapidjson/filewritestream.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <iostream>

namespace Pokemon
{
	void SaveSpecieData(const int nationalDexNumber, const SpeciesData& data)
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
	
		d.SetObject();
		d.AddMember("data_version", JSON_DATA_VERSION, d.GetAllocator());
	
		rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
		d.Accept(writer);
	
		fclose(fp);	
	}
	
	void LoadSpecieData(const int nationalDexNumber, SpeciesData& data)
	{
		if (nationalDexNumber <= 0 || nationalDexNumber >= 1008)
		{
			// Isn't a valid national dex number
			data.nationalDexNumber = -1;
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
			data.nationalDexNumber = -1;
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
	}
}