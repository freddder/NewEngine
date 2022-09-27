#include "cLightManager.h"
#include <sstream>
#include <glad/glad.h>

cLightManager::cLightManager()
{
	glGenBuffers(1, &uboLights);

	glBindBuffer(GL_UNIFORM_BUFFER, uboLights);
	glBufferData(GL_UNIFORM_BUFFER, NUMBER_OF_LIGHTS * sizeof(sLight), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboLights, 0, NUMBER_OF_LIGHTS * sizeof(sLight));
}

cLightManager::~cLightManager()
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
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
