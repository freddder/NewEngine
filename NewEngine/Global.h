#ifndef _globals_HG_
#define _globals_HG_

//#include <vector>

#include "cCamera.h"
#include "cModel.h"
#include "cShaderManager.h"
#include "cModelManager.h"
#include "cTextureManager.h"
#include "cLightManager.h"
#include "cAnimationManager.h"
#include "cMapManager.h"
#include <set>

void StartUp();
void Shutdown();

enum eDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

extern cCamera* g_Camera;

extern cShaderManager* g_ShaderManager;

extern cModelManager* g_ModelManager;

extern cTextureManager* g_TextureManager;

extern cLightManager* g_LightManager;

extern cAnimationManager* g_AnimationManager;

extern cMapManager* g_MapManager;

extern std::set< cModel* > g_set_Models;

#endif // _globalThings_HG_