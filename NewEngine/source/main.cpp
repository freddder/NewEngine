#include <glad/glad.h>
#include <iostream>

#include "Engine.h"
#include "cSceneManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"
#include "cUIManager.h"
#include "cCameraManager.h"

#include "cWildRoamingPokemon.h"
#include "cTamedRoamingPokemon.h"
#include "cPlayerEntity.h"

#include "Player.h"

int main()
{
    if (!Engine::InitializeGLFW()) return -1;

    Engine::StartUpManagers();

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // Enable transparency objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //********************** Prepare Light **************************************

    Manager::light.lights[0].extraParam.x = 2.f; // directional light
    Manager::light.lights[0].position = glm::vec4(-10.f, 10.f, 17.f, 1.f);
    Manager::light.lights[0].direction = -(Manager::light.lights[0].position);
    Manager::light.lights[0].diffuse = glm::vec4(1.f, 0.717f, 0.43f, 1.f);
    Manager::light.lights[0].extraParam.w = 1.f; // turn on

    // Position
    std::shared_ptr<cFloatAnimation> lightPosAnim = std::make_shared<cFloatAnimation>(Manager::light.lights[0].position.z);
    lightPosAnim->AddKeyFrame(sKeyFrameFloat(60.f, -17.f));
    lightPosAnim->speed = 5.f;
    lightPosAnim->isRepeat = true;
    //Manager::animation.AddAnimation(lightPosAnim);

    // Color
    std::shared_ptr<cVec4Animation> lightColorAnim = std::make_shared<cVec4Animation>(Manager::light.lights[0].diffuse);
    lightColorAnim->AddKeyFrame(sKeyFrameVec4(30.f, glm::vec4(1.f, 1.f, 1.f, 1.f)));
    lightColorAnim->AddKeyFrame(sKeyFrameVec4(60.f, glm::vec4(0.12f, 0.22f, 0.5f, 1.f)));
    lightColorAnim->speed = 5.f;
    lightColorAnim->isRepeat = true;
    //Manager::animation.AddAnimation(lightColorAnim);

    Manager::animation.InitializeAnimationsPresets();

    //Manager::scene.SetWeather(SNOW);

    Engine::GameLoop(true);

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}