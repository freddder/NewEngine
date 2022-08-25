#pragma once
#include <map>

#include "DrawInfo.h"

const std::string MODEL_PATH = "assets/models/";

class cModelManager
{
public:
	cModelManager();
	~cModelManager();

	bool LoadModel(std::string fileName, std::string programName);
	bool FindModelByName(std::string fileName, sModelDrawInfo& modelInfo);
	std::string GetLastError(bool clear);

private:
	
	std::map<std::string, sModelDrawInfo> modelMap; // stored by file name
	std::string lastError;

	void CreateModelVAOs(sModelDrawInfo& newModel, unsigned int program);
};