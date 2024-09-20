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

    // Setup shader programs
    Manager::render.CreateShadderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    Manager::render.CreateShadderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    Manager::render.CreateShadderProgram("sprite", "SpriteVertShader.glsl", "FragShader1.glsl");
    Manager::render.CreateShadderProgram("wave", "WaveVertShader.glsl", "WaveFragShader.glsl");
    Manager::render.CreateShadderProgram("ocean", "OceanVertShader.glsl", "OceanFragShader.glsl");
    Manager::render.CreateShadderProgram("foam", "FoamVertShader.glsl", "FoamFragShader.glsl");
    Manager::render.CreateShadderProgram("tree", "TreeVertShader.glsl", "FragShader1.glsl");
    Manager::render.CreateShadderProgram("snow", "SnowVertShader.glsl", "SnowFragShader.glsl");
    Manager::render.CreateShadderProgram("particle", "3DParticleVertShader.glsl", "FragShader1.glsl");
    Manager::render.CreateShadderProgram("ui", "UIVertShader.glsl", "UIFragShader.glsl");
    Manager::render.CreateShadderProgram("text", "TextVertShader.glsl", "TextFragShader.glsl");

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

    //********************** Load models, textures and fonts ***************************

    Manager::render.LoadModel("SpriteHolder.obj", "sprite");
    Manager::render.LoadModel("SpriteHolder.obj", "snow");
    Manager::render.LoadModel("Water_c2.obj", "wave");
    Manager::render.LoadModel("Water_b2.obj", "wave");
    Manager::render.LoadModel("Water_bl2.obj", "wave");
    Manager::render.LoadModel("sea_water2.obj", "ocean");
    Manager::render.LoadModel("Foam_b2.obj", "foam");
    Manager::render.LoadModel("Foam_bl2.obj", "foam");
    Manager::render.LoadModel("Foam_c2.obj", "foam");
    Manager::render.LoadModel("Foam_c2.obj", "foam");
    Manager::render.LoadModel("r0_treePine.obj", "tree");

    Manager::render.LoadSpriteSheet("Nate.png", 3, 8, false);
    Manager::render.LoadSpriteSheet("SymetricNPC_1.png", 2, 4, true);
    Manager::render.LoadSpriteSheet("AsymetricalNPC_1.png", 3, 4, false);
    Manager::render.LoadSpriteSheet("722.png", 4, 4, false);

    Manager::render.LoadTexture("SnowFlake1.png");
    Manager::render.LoadTexture("SnowFlake2.png");
    Manager::render.LoadTexture("SnowFlake3.png");

    //***************************************************************************

    Manager::animation.InitializeAnimationsPresets();

    //Manager::map.LoadMap("WinterTestDesc.json");
    //Manager::map.LoadMap("WaterTest3Desc.json");
    //Manager::map.LoadMap("SlopeTestDesc.json");
    //Manager::map.LoadMap("MultiTestDesc.json");
    Manager::map.LoadScene("GrassRouteDemoDesc.json");
    //Manager::map.LoadMap("CostalWinterDesc.json");

    Pokemon::sIndividualData partner;
    partner.nationalDexNumber = 445;
    partner.gender = Pokemon::FEMALE;
    partner.isShiny = true;
    partner.form.battleBackSpriteFrameCount = 48; // hard coded for now
    partner.LoadFormFromSpeciesData();
    Player::AddPartyMember(partner);

    Manager::scene.LoadSpawnData(406, 0, 0, Pokemon::TALL_GRASS, 0, "");
    Manager::scene.LoadSpawnData(678, 0, 0, Pokemon::TALL_GRASS, 0, "");

    {
        Player::playerChar = new cPlayerEntity(glm::vec3(23.f, 1.f, 25.f));
        Manager::camera.targetPosRef = Player::GetPlayerPositionRef();

        Pokemon::sSpeciesData followerSpecieData;
        Pokemon::LoadSpecieData(Player::party[0].nationalDexNumber, followerSpecieData);
        Manager::render.LoadRoamingPokemonSpecieTextures(followerSpecieData);

        Player::playerPartner = Manager::scene.SpawnTamedPokemon(Player::party[0], glm::vec3(22.f, 1.f, 25.f));
        Player::playerChar->SetFollower(Player::playerPartner.get());

        // TODO: find a good place to seed the rand
        srand((int)time(0));
        Manager::scene.SpawnRandomWildPokemon();
        Manager::scene.SpawnRandomWildPokemon();
        Manager::scene.SpawnRandomWildPokemon();
        Manager::scene.SpawnRandomWildPokemon();
        Manager::scene.SpawnRandomWildPokemon();
    }

    //Manager::scene.SetWeather(SNOW);

    Engine::GameLoop(true);

    delete Player::playerChar;

    Engine::ShutdownManagers();

    Engine::ShutdownGLFW();

    return 0;
}