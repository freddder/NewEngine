#include <iostream>

#include "cSceneManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"

#include "cCamera.h"
#include "cOverworldPokemon.h"
#include "cPlayerCharacter.h"

#include "Engine.h"

int main()
{
    if (!Engine::InitializeGLFW()) return -1;

    Engine::StartUpManagers();

    cCamera* camera = cCamera::GetInstance();
    cLightManager* lightManager = cLightManager::GetInstance();
    cAnimationManager* animationManager = cAnimationManager::GetInstance();
    cRenderManager* renderManager = cRenderManager::GetInstance();
    cMapManager* mapManager = cMapManager::GetInstance();
    cSceneManager* sceneManager = cSceneManager::GetInstance();

    // Setup shader programs
    renderManager->CreateShadderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    renderManager->CreateShadderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    renderManager->CreateShadderProgram("sprite", "SpriteVertShader.glsl", "FragShader1.glsl");
    renderManager->CreateShadderProgram("wave", "WaveVertShader.glsl", "WaveFragShader.glsl");
    renderManager->CreateShadderProgram("ocean", "OceanVertShader.glsl", "OceanFragShader.glsl");
    renderManager->CreateShadderProgram("foam", "FoamVertShader.glsl", "FoamFragShader.glsl");
    renderManager->CreateShadderProgram("snow", "SnowVertShader.glsl", "SnowFragShader.glsl");
    renderManager->CreateShadderProgram("particle", "3DParticleVertShader.glsl", "FragShader1.glsl");
    renderManager->CreateShadderProgram("debug", "DebugVertShader.glsl", "DebugFragShader.glsl");

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    // Enable transparency objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //********************** Prepare Light **************************************

    lightManager->lights[0].extraParam.x = 2.f; // directional light
    lightManager->lights[0].position = glm::vec4(-10.f, 10.f, 10.f, 1.f);
    lightManager->lights[0].direction = -(lightManager->lights[0].position);
    lightManager->lights[0].extraParam.w = 1.f; // turn on

    //********************** Load models and textures ***************************

    renderManager->LoadModel("r0_treePine.obj", "scene");
    renderManager->LoadModel("WaterTest3.obj", "scene");
    renderManager->LoadModel("MultiTest.obj", "scene");
    renderManager->LoadModel("GrassRouteDemo.obj", "scene");
    renderManager->LoadModel("SpriteHolder.obj", "sprite");
    renderManager->LoadModel("SpriteHolder.obj", "snow");
    renderManager->LoadModel("Water_c2.obj", "wave");
    renderManager->LoadModel("Water_b2.obj", "wave");
    renderManager->LoadModel("Water_bl2.obj", "wave");
    renderManager->LoadModel("sea_water2.obj", "ocean");
    renderManager->LoadModel("Foam_b2.obj", "foam");
    renderManager->LoadModel("Foam_bl2.obj", "foam");
    renderManager->LoadModel("Foam_c2.obj", "foam");

    renderManager->LoadSpriteSheet("Nate.png", 3, 8, false);
    renderManager->LoadSpriteSheet("SymetricNPC_1.png", 2, 4, true);
    renderManager->LoadSpriteSheet("AsymetricalNPC_1.png", 3, 4, false);
    renderManager->LoadSpriteSheet("722.png", 4, 4, false);

    renderManager->LoadSceneTexture("SnowFlake1.png");
    renderManager->LoadSceneTexture("SnowFlake2.png");
    renderManager->LoadSceneTexture("SnowFlake3.png");

    renderManager->LoadSceneTexture("PartyMemberButtonBackground.png", "ui/", false);
    renderManager->LoadSceneTexture("ico_3ds_646-white.png", "ui/PokemonPartySprites/", false);

    animationManager->InitializeAnimationsPresets();

    // UI testing
    {
        cUICanvas* canvas = new cUICanvas();
        Engine::button = new cUIStaticImage();
        Engine::button->anchor = MIDDLE_LEFT;
        Engine::button->textureName = "PartyMemberButtonBackground.png";
        Engine::button->aspectRatio = 0.365f;
        Engine::button->heightPercent = 1.f / 9.f;

        Engine::sprite = new cUIStaticImage();
        Engine::sprite->anchor = MIDDLE_LEFT;
        Engine::sprite->aspectRatio = 3.f / 4.f;
        Engine::sprite->heightPercent = 3.f / 4.f;
        Engine::sprite->textureName = "ico_3ds_722.png";

        // Make sure to add back gound items last
        Engine::button->AddChild(Engine::sprite);

        canvas->AddWidget(Engine::button);
        cUIManager::GetInstance()->AddCanvas(canvas);
    }

    Engine::playerChar = new cPlayerCharacter(glm::vec3(0.f, 1.f, 5.f));
    cOverworldPokemon* follower = new cOverworldPokemon(glm::vec3(0.f, 1.f, 4.f), "722.png");
    Engine::playerChar->SetFollower(follower);
    camera->playerPosition = &Engine::playerChar->model->position;

    mapManager->LoadMap("WinterTestDesc.json");
    //mapManager->LoadMap("WaterTest3Desc.json");
    //mapManager->LoadMap("SlopeTestDesc.json");
    //mapManager->LoadMap("MultiTestDesc.json");
    //mapManager->LoadMap("GrassRouteDemoDesc.json");

    //weatherManager->SetWeather(SNOW);

    Engine::GameLoop(true);

    delete Engine::playerChar;
    delete follower;

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}