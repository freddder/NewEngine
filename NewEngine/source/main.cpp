#include <iostream>

#include "cSceneManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"
#include "cUIManager.h"

#include "cCamera.h"
#include "cOverworldPokemon.h"
#include "cPlayerCharacter.h"

#include "Engine.h"
#include "Player.h"

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
    renderManager->CreateShadderProgram("tree", "TreeVertShader.glsl", "FragShader1.glsl");
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
    lightManager->lights[0].position = glm::vec4(-10.f, 10.f, 17.f, 1.f);
    lightManager->lights[0].direction = -(lightManager->lights[0].position);
    lightManager->lights[0].diffuse = glm::vec4(1.f, 0.717f, 0.43f, 1.f);
    lightManager->lights[0].extraParam.w = 1.f; // turn on

    // Position
    std::shared_ptr<cFloatAnimation> lightPosAnim = std::make_shared<cFloatAnimation>(lightManager->lights[0].position.z);
    lightPosAnim->AddKeyFrame(sKeyFrameFloat(60.f, -17.f));
    lightPosAnim->speed = 5.f;
    lightPosAnim->isRepeat = true;
    //cAnimationManager::GetInstance()->AddAnimation(lightPosAnim);

    // Color
    std::shared_ptr<cVec4Animation> lightColorAnim = std::make_shared<cVec4Animation>(lightManager->lights[0].diffuse);
    lightColorAnim->AddKeyFrame(sKeyFrameVec4(30.f, glm::vec4(1.f, 1.f, 1.f, 1.f)));
    lightColorAnim->AddKeyFrame(sKeyFrameVec4(60.f, glm::vec4(0.12f, 0.22f, 0.5f, 1.f)));
    lightColorAnim->speed = 5.f;
    lightColorAnim->isRepeat = true;
    //cAnimationManager::GetInstance()->AddAnimation(lightColorAnim);

    //********************** Load models and textures ***************************

    //renderManager->LoadModel("WaterTest3.obj", "scene");
    //renderManager->LoadModel("MultiTest.obj", "scene");
    //renderManager->LoadModel("GrassRouteDemo.obj", "scene");
    //renderManager->LoadModel("CostalWinter.obj", "scene");
    //renderManager->LoadModel("WinterTest.obj", "scene");
    //renderManager->LoadModel("SlopeTest.obj", "scene");
    renderManager->LoadModel("SpriteHolder.obj", "sprite");
    renderManager->LoadModel("SpriteHolder.obj", "snow");
    renderManager->LoadModel("Water_c2.obj", "wave");
    renderManager->LoadModel("Water_b2.obj", "wave");
    renderManager->LoadModel("Water_bl2.obj", "wave");
    renderManager->LoadModel("sea_water2.obj", "ocean");
    renderManager->LoadModel("Foam_b2.obj", "foam");
    renderManager->LoadModel("Foam_bl2.obj", "foam");
    renderManager->LoadModel("Foam_c2.obj", "foam");
    renderManager->LoadModel("Foam_c2.obj", "foam");
    renderManager->LoadModel("r0_treePine.obj", "tree");

    renderManager->LoadSpriteSheet("Nate.png", 3, 8, false);
    renderManager->LoadSpriteSheet("SymetricNPC_1.png", 2, 4, true);
    renderManager->LoadSpriteSheet("AsymetricalNPC_1.png", 3, 4, false);
    renderManager->LoadSpriteSheet("722.png", 4, 4, false);

    renderManager->LoadSceneTexture("SnowFlake1.png");
    renderManager->LoadSceneTexture("SnowFlake2.png");
    renderManager->LoadSceneTexture("SnowFlake3.png");

    renderManager->LoadSceneTexture("PartyMemberButtonBackground.png", "ui/", false);
    renderManager->LoadSceneTexture("ico_3ds_722.png", "ui/PokemonPartySprites/", false);

    animationManager->InitializeAnimationsPresets();

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

    //mapManager->LoadMap("WinterTestDesc.json");
    //mapManager->LoadMap("WaterTest3Desc.json");
    //mapManager->LoadMap("SlopeTestDesc.json");
    //mapManager->LoadMap("MultiTestDesc.json");
    mapManager->LoadMap("GrassRouteDemoDesc.json");
    //mapManager->LoadMap("CostalWinterDesc.json");

    sceneManager->LoadSpawnData(678, 0, 0, 0, "");

    {
        Player::playerChar = new cPlayerCharacter(glm::vec3(23.f, 1.f, 25.f));
        std::shared_ptr<cOverworldPokemon> follower = sceneManager->CreateRoamingWildPokemon(0, glm::vec3(22.f, 1.f, 25.f));
        Player::playerChar->SetFollower(follower.get());
        camera->targetPosRef = Player::GetPlayerPositionRef();
    }

    //sceneManager->SetWeather(SNOW);

    Engine::GameLoop(true);

    delete Player::playerChar;

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}