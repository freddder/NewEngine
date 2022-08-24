#include "cRenderModel.h"
#include <glad/glad.h>
#include "Global.h"

cRenderModel::cRenderModel()
{
	position = glm::vec3(0.f);
	orientation = glm::vec3(0.f);
	scale = glm::vec3(1.f);

	isWireframe = false;
	isInstanced = false;
	useWholeColor = false;

	wholeColor = glm::vec4(1.f, 1.f, 1.f, 1.f);

	textureName = "";

	shaderName = "scene";
	textureAnimationType = None;
	//currSpriteId = 0;
	useGlobalPosForUV = false;
	globalUVRatios = glm::vec2(1.f);
	textureOffset = glm::vec3(0.f);
}

void cRenderModel::InstanceObject(std::vector<glm::vec4>& offsets, unsigned int program)
{
	isInstanced = true;
	instancedNum = offsets.size();

	// Generate offsets buffer
	glGenBuffers(1, &(instanceOffsetsBufferId));

	glBindBuffer(GL_ARRAY_BUFFER, instanceOffsetsBufferId);

	glBufferData(GL_ARRAY_BUFFER,
		sizeof(glm::vec4) * offsets.size(),
		(GLvoid*)&offsets[0],
		GL_STATIC_DRAW);

	GLint offset_location = glGetAttribLocation(program, "oOffset");
	glEnableVertexAttribArray(offset_location);
	glVertexAttribPointer(offset_location, 4,
		GL_FLOAT, GL_FALSE,
		sizeof(glm::vec4),
		(void*)0);
	glVertexAttribDivisor(offset_location, 1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableVertexAttribArray(offset_location);
}

void cRenderModel::SetUpUniforms()
{
	unsigned int textureId;
	g_TextureManager->GetTexureId(textureName, textureId);

	GLuint textureUnit = 0;			// Texture unit go from 0 to 79
    glActiveTexture(textureUnit + GL_TEXTURE0);	// GL_TEXTURE0 = 33984
    glBindTexture(GL_TEXTURE_2D, textureId);

    g_RenderManager->setInt("texture_00", textureUnit);
}
