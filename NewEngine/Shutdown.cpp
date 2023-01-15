#include "Global.h"

void Shutdown()
{
	delete g_Camera;

	delete g_ModelManager;

	delete g_TextureManager;

	delete g_LightManager;

	delete g_AnimationManager;

	//delete g_MapManager;

	//delete g_RenderManager;

	//delete g_WeatherManager;

	//delete g_ParticleManager;
}