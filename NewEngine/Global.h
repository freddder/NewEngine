#ifndef _globals_HG_
#define _globals_HG_

//#include <vector>

#include "cCamera.h"
#include "cModelManager.h"
#include "cTextureManager.h"
#include "cLightManager.h"
#include "cAnimationManager.h"
#include "cMapManager.h"
#include "cRenderManager.h"
#include <set>

void StartUp();
void Shutdown();

extern cCamera* g_Camera;

extern cModelManager* g_ModelManager;

extern cTextureManager* g_TextureManager;

extern cLightManager* g_LightManager;

extern cAnimationManager* g_AnimationManager;

extern cMapManager* g_MapManager;

extern cRenderManager* g_RenderManager;

#endif // _globalThings_HG_