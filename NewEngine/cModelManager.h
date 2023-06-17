#pragma once
#include <map>

#include "DrawInfo.h"

//const std::string MODEL_PATH = "assets/models/";

class cModelManager
{
	static cModelManager* singleton;
	cModelManager();
	~cModelManager();
	cModelManager(const cModelManager& obj) = delete;

	std::map<std::string, sModelDrawInfo> modelMap; // stored by file name
	std::string lastError;

	void CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program);

public:

	static cModelManager* GetInstance()
	{
		if (singleton == NULL)
		{
			singleton = new cModelManager();
		}

		return singleton;
	}

	static void DestroyInstance()
	{
		if (singleton != NULL)
		{
			delete singleton;
			singleton = NULL;
		}
	}

	bool LoadModel(std::string fileName, std::string programName);
	bool FindModelByName(std::string fileName, sModelDrawInfo& modelInfo);
	std::string GetLastError(bool clear);
};