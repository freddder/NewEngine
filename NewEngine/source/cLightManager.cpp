#include "cLightManager.h"
#include <sstream>
#include <glad/glad.h>

cLightManager::cLightManager()
{
}

cLightManager::~cLightManager()
{	
}

void cLightManager::Startup()
{
	glGenBuffers(1, &uboLights);

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, (NUMBER_OF_LIGHTS * sizeof(sLight)) + sizeof(GL_INT), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, NUMBER_OF_LIGHTS * sizeof(sLight));

	shadowSampleRadius = 5;
}

void cLightManager::Shutdown()
{
	glDeleteBuffers(1, &uboLights);
}

void cLightManager::AddProgramToBlock(unsigned int newProgram)
{
	unsigned int uniformBlockIndex = glGetUniformBlockIndex(newProgram, "Lights");
	glUniformBlockBinding(newProgram, uniformBlockIndex, 1);
}

void cLightManager::SetUnimormValues()
{
	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, NUMBER_OF_LIGHTS * sizeof(sLight), lights);
	glBufferSubData(GL_UNIFORM_BUFFER, NUMBER_OF_LIGHTS * sizeof(sLight), sizeof(GL_INT), &shadowSampleRadius);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
