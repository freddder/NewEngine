#include "Global.h"

void StartUp()
{
	g_Camera = new cCamera();

	//g_ShaderManager = new cShaderManager();//("VertShader1.glsl", "FragShader1.glsl");

	g_ModelManager = new cModelManager();

	g_TextureManager = new cTextureManager();

	g_LightManager = new cLightManager();

	g_AnimationManager = new cAnimationManager();

	//g_MapManager = new cMapManager();

	g_RenderManager = new cRenderManager();

	g_MapManager = new cMapManager();
}