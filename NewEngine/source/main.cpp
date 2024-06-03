#include <iostream>

#include "cSceneManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"
#include "cUIManager.h"

#include "cCamera.h"
#include "cWildRoamingPokemon.h"
#include "cTamedRoamingPokemon.h"
#include "cPlayerEntity.h"

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
    renderManager->CreateShadderProgram("ui", "UIVertShader.glsl", "UIFragShader.glsl");
    renderManager->CreateShadderProgram("text", "TextVertShader.glsl", "TextFragShader.glsl");

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

    //********************** Load models, textures and fonts ***************************

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
    
    renderManager->LoadFont("Truth And Ideals-Normal.ttf", 24);

    //***************************************************************************

    animationManager->InitializeAnimationsPresets();

    {
        cUICanvas* canvas = new cUICanvas();
        Engine::button = new cUIStaticImage();
        Engine::button->anchor = MIDDLE_LEFT;
        Engine::button->textureName = "PartyMemberButtonBackground.png";
        Engine::button->aspectRatio = 0.365f;
        Engine::button->heightPercent = 1.f / 9.f;

        //Engine::sprite = new cUIStaticImage();
        //Engine::sprite->anchor = MIDDLE_LEFT;
        //Engine::sprite->aspectRatio = 3.f / 4.f;
        //Engine::sprite->heightPercent = 3.f / 4.f;
        //Engine::sprite->textureName = "ico_3ds_722.png";

        // Make sure to add back gound items last
        //Engine::button->AddChild(Engine::sprite);

        renderManager->testWidget = new cUIText(Engine::button);
        renderManager->testWidget->fontName = "Truth And Ideals-Normal.ttf";
        renderManager->testWidget->text = "Testing, stuff!";
        renderManager->testWidget->color = glm::vec3(1.f);
        renderManager->testWidget->heightPercent = 0.34f;
        renderManager->testWidget->origin.y = 0.5f;
        renderManager->testWidget->origin.x = 0.5f;
        renderManager->CreateTextDataBuffer(renderManager->testWidget);

        canvas->AddWidget(Engine::button);
        cUIManager::GetInstance()->AddCanvas(canvas);
    }

    //mapManager->LoadMap("WinterTestDesc.json");
    //mapManager->LoadMap("WaterTest3Desc.json");
    //mapManager->LoadMap("SlopeTestDesc.json");
    //mapManager->LoadMap("MultiTestDesc.json");
    mapManager->LoadMap("GrassRouteDemoDesc.json");
    //mapManager->LoadMap("CostalWinterDesc.json");

    sceneManager->LoadSpawnData(406, 0, 0, Pokemon::TALL_GRASS, 0, "");
    sceneManager->LoadSpawnData(678, 0, 0, Pokemon::TALL_GRASS, 0, "");

    {
        Player::playerChar = new cPlayerEntity(glm::vec3(23.f, 1.f, 25.f));
        camera->targetPosRef = Player::GetPlayerPositionRef();

        Pokemon::sSpeciesData followerSpecieData;
        Pokemon::LoadSpecieData(445, followerSpecieData);
        renderManager->LoadRoamingPokemonSpecieSpriteSheets(followerSpecieData);
        Player::partyMember1.nationalDexNumber = 445;
        Player::partyMember1.gender = Pokemon::MALE;
        Player::partyMember1.isShiny = true;
        std::shared_ptr<cTamedRoamingPokemon> follower = sceneManager->SpawnTamedPokemon(Player::partyMember1, glm::vec3(22.f, 1.f, 25.f));
        Player::playerChar->SetFollower(follower.get());

        // TODO: find a good place to seed the rand
        srand((int)time(0));
        sceneManager->SpawnRandomWildPokemon();
        sceneManager->SpawnRandomWildPokemon();
        sceneManager->SpawnRandomWildPokemon();
        sceneManager->SpawnRandomWildPokemon();
        sceneManager->SpawnRandomWildPokemon();
    }

    //sceneManager->SetWeather(SNOW);

    Engine::GameLoop(true);

    delete Player::playerChar;
    delete renderManager->testWidget;

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}