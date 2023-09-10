#include "Global.h"

#include "cCamera.h"
#include "cTextureManager.h"
#include "cLightManager.h"
#include "cAnimationManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cWeatherManager.h"
#include "cParticleManager.h"

void Shutdown()
{
    cCamera::DestroyInstance();

    cTextureManager::DestroyInstance();

    cLightManager::DestroyInstance();

    cAnimationManager::DestroyInstance();

    cRenderManager::DestroyInstance();

    cMapManager::DestroyInstance();

    cWeatherManager::DestroyInstance();

    cParticleManager::DestroyInstance();
}