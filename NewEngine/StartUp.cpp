#include "Global.h"

void StartUp()
{
	g_Camera = new cCamera();

	g_ModelManager = new cModelManager();

	g_TextureManager = new cTextureManager();

	g_LightManager = new cLightManager();

	g_AnimationManager = new cAnimationManager();

	g_RenderManager = new cRenderManager();

	g_MapManager = new cMapManager();

	g_WeatherManager = new cWeatherManager();
}