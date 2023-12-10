#include <iostream>

#include "cWeatherManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"
#include "cTextureManager.h"

#include "cCamera.h"
#include "cOverworldPokemon.h"
#include "cPlayerCharacter.h"

#include "Engine.h"
#include "cUIManager.h"

int main()
{
    if (!Engine::InitializeGLFW()) return -1;

    Engine::StartUpManagers();

    cCamera* camera = cCamera::GetInstance();
    cTextureManager* textureManager = cTextureManager::GetInstance();
    cLightManager* lightManager = cLightManager::GetInstance();
    cAnimationManager* animationManager = cAnimationManager::GetInstance();
    cRenderManager* renderManager = cRenderManager::GetInstance();
    cMapManager* mapManager = cMapManager::GetInstance();
    cWeatherManager* weatherManager = cWeatherManager::GetInstance();

    // Setup shader programs
    renderManager->CreateShadderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    renderManager->CreateShadderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    renderManager->CreateShadderProgram("sprite", "SpriteVertShader.glsl", "SpriteFragShader.glsl");
    renderManager->CreateShadderProgram("wave", "WaveVertShader.glsl", "WaveFragShader.glsl");
    renderManager->CreateShadderProgram("ocean", "OceanVertShader.glsl", "OceanFragShader.glsl");
    renderManager->CreateShadderProgram("foam", "FoamVertShader.glsl", "FoamFragShader.glsl");
    //renderManager->CreateShadderProgram("2dsnow", "2DSnowVertShader.glsl", "2DSnowFragShader.glsl");
    renderManager->CreateShadderProgram("snow", "SnowVertShader.glsl", "SnowFragShader.glsl");
    renderManager->CreateShadderProgram("particle", "3DParticleVertShader.glsl", "3DParticleFragShader.glsl");
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
    renderManager->LoadModel("WinterTest.obj", "scene");
    renderManager->LoadModel("SlopeTest.obj", "scene");
    //modelManager->LoadModel("TestMapWater.obj", "scene");
    //modelManager->LoadModel("FallTree.obj", "scene");
    renderManager->LoadModel("SpriteHolder.obj", "sprite");
    renderManager->LoadModel("SpriteHolder.obj", "snow");
    renderManager->LoadModel("Water_c2.obj", "wave");
    renderManager->LoadModel("Water_b2.obj", "wave");
    renderManager->LoadModel("Water_bl2.obj", "wave");
    renderManager->LoadModel("sea_water2.obj", "ocean");
    renderManager->LoadModel("Foam_b2.obj", "foam");
    renderManager->LoadModel("Foam_bl2.obj", "foam");
    renderManager->LoadModel("Foam_c2.obj", "foam");

    textureManager->CreateSpriteSheet("Nate.png", 3, 8, false);
    textureManager->CreateSpriteSheet("SymetricNPC_1.png", 2, 4, true);
    textureManager->CreateSpriteSheet("AsymetricalNPC_1.png", 3, 4, false);
    textureManager->CreateSpriteSheet("722.png", 4, 4, false);

    textureManager->CreateTexture("SnowFlake1.png");
    textureManager->CreateTexture("SnowFlake2.png");
    textureManager->CreateTexture("SnowFlake3.png");

    textureManager->CreateTexture("PartyMemberButtonBackground.png", "ui/");
    textureManager->CreateTexture("ico_3ds_646-white.png", "ui/PokemonPartySprites/");

    animationManager->InitializeAnimationsPresets();

    //cRenderModel* tree = new cRenderModel();
    //tree->meshName = "FallTree.obj";
    //tree->position.x = -0.5f;
    //tree->position.z = -0.5f;
    //g_RenderManager->AddModel(tree);

    cUICanvas canvas;
    cUIWidget button;
    button.aspectRatio = 0.365f;
    button.heightPercent = 1.f / 9.f;

    cUIStaticImage background;
    background.textureName = "PartyMemberButtonBackground.png";
    background.aspectRatio = 0.365f;
    background.heightPercent = 1.f;

    cUIStaticImage sprite;
    sprite.aspectRatio = 3.f / 4.f;
    sprite.heightPercent = 3.f / 4.f;
    sprite.textureName = "ico_3ds_646-white.png";

    button.AddChild(background);
    button.AddChild(sprite);
    canvas.AddWidget(button);
    cUIManager::GetInstance()->AddCanvas(canvas);

    Engine::playerChar = new cPlayerCharacter(glm::vec3(0.f, 0.f, 2.f));
    cOverworldPokemon* follower = new cOverworldPokemon(glm::vec3(0.f, 0.f, 3.f), "722.png");
    Engine::playerChar->SetFollower(follower);
    camera->playerPosition = &Engine::playerChar->model->position;

    //mapManager->LoadMap("WinterTestDesc.json");
    mapManager->LoadMap("WaterTest3Desc.json");
    //mapManager->LoadMap("SlopeTestDesc.json");

    weatherManager->SetWeather(SNOW);

    Engine::GameLoop(true);

    delete Engine::playerChar;
    delete follower;

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}