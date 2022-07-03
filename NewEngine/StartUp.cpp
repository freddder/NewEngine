#include "Global.h"

void StartUp()
{
	g_Camera = new cCamera();

	g_ShaderManager = new cShaderManager();//("VertShader1.glsl", "FragShader1.glsl");

	g_ModelManager = new cModelManager();

	g_TextureManager = new cTextureManager();

	g_LightManager = new cLightManager();
}