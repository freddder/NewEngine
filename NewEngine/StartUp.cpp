#include "Global.h"

#include "cCamera.h"
//#include "cModelManager.h"
#include "cTextureManager.h"
#include "cLightManager.h"
#include "cAnimationManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cWeatherManager.h"
#include "cParticleManager.h"

void StartUp()
{
    cCamera::GetInstance();

    //cModelManager::GetInstance();

    cTextureManager::GetInstance();

    cLightManager::GetInstance();

    cAnimationManager::GetInstance();

    cRenderManager::GetInstance();

    cMapManager::GetInstance();

    cWeatherManager::GetInstance();

    cParticleManager::GetInstance();
}