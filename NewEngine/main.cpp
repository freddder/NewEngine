#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Global.h"

#include <iostream>
#include "cSpriteAnimation.h"
#include "cFloatAnimation.h"
#include "cModelAnimation.h"
#include "cPlayerCharacter.h"
#include "cSpriteModel.h"

#include "cParticleManager.h"
#include "cWeatherManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cAnimationManager.h"
#include "cLightManager.h"
#include "cTextureManager.h"
//#include "cModelManager.h"
#include "cCamera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// camera
float lastX = 1200 / 2.0f;
float lastY = 640 / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

cPlayerCharacter* playerChar;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(1200, 640, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    StartUp();

    cCamera* camera = cCamera::GetInstance();
    //cModelManager* modelManager = cModelManager::GetInstance();
    cTextureManager* textureManager = cTextureManager::GetInstance();
    cLightManager* lightManager = cLightManager::GetInstance();
    cAnimationManager* animationManager = cAnimationManager::GetInstance();
    cRenderManager* renderManager = cRenderManager::GetInstance();
    cMapManager* mapManager = cMapManager::GetInstance();
    cWeatherManager* weatherManager = cWeatherManager::GetInstance();
    cParticleManager* particleManager = cParticleManager::GetInstance();

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
    //renderManager->LoadModel("SpriteHolder.obj", "particle");
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

    textureManager->CreateTexture("SnowFlake1.png");
    textureManager->CreateTexture("SnowFlake2.png");
    textureManager->CreateTexture("SnowFlake3.png");

    //cRenderModel* tree = new cRenderModel();
    //tree->meshName = "FallTree.obj";
    //tree->position.x = -0.5f;
    //tree->position.z = -0.5f;
    ////g_set_Models.insert(tree);
    //g_RenderManager->AddModel(tree);

    //cSpriteModel* sprite = new cSpriteModel();
    //sprite->meshName = "SpriteHolder.obj";
    //sprite->textureName = "Nate.png";
    //sprite->currSpriteId = 3;
    //renderManager->AddModel(sprite);
    //
    //cSpriteAnimation* spriteAnimation = new cSpriteAnimation(sprite->currSpriteId, sprite->scale);
    //spriteAnimation->AddKeyFrames(KEYFRAMES_WALK_RIGHT_R);
    //spriteAnimation->isRepeat = true;
    //animationManager->AddAnimation(spriteAnimation);

    //cSpriteModel* spriteSym = new cSpriteModel();
    //spriteSym->meshName = "SpriteHolder.obj";
    //spriteSym->position.z = -2.f;
    //spriteSym->textureName = "SymetricNPC_1.png";
    //spriteSym->currSpriteId = 2;
    //renderManager->AddModel(spriteSym);
    //
    //cSpriteAnimation* symSpriteAnimation = new cSpriteAnimation(spriteSym->currSpriteId, spriteSym->scale);
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 3));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.4f, 2));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.6f, 3, true));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.8f, 2));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.0f, 1));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.2f, 0));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.4f, 1, true));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.6f, 0));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.8f, 5));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.0f, 4));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.2f, 6));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.4f, 4));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.6f, 5, true));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.8f, 4, true));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(3.0f, 6, true));
    //symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(3.2f, 4, true));
    //symSpriteAnimation->isRepeat = true;
    //animationManager->AddAnimation(symSpriteAnimation);

    playerChar = new cPlayerCharacter(glm::vec3(0.f, 0.f, 2.f));
    camera->playerPosition = &playerChar->model->position;

    //mapManager->LoadMap("WinterTestDesc.json");
    //mapManager->LoadMap("WinterTestDesc.json");
    mapManager->LoadMap("SlopeTestDesc.json");

    weatherManager->SetWeather(SNOW);

    //********************** Setup on screen texture ****************************

    unsigned int quadVAO;
    unsigned int quadVBO;

    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    float noiseTimer = 0.f;
    float waterThreshold = 0.7;

    //***************************************************************************

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.WantCaptureMouse = false;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        lightManager->lights[0].direction = -(lightManager->lights[0].position);

        animationManager->Process(deltaTime);

        particleManager->UpdateSpawners(deltaTime);

        renderManager->DrawScene();

        //renderManager->use("debug");

        //noiseTimer += deltaTime;
        //renderManager->setFloat("timer", noiseTimer);
        //renderManager->setFloat("threshold", waterThreshold);

        //glBindVertexArray(quadVAO);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glBindVertexArray(0);

        //**************************************************************************

        // Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("General");
        ImGui::Text("FPS: %f", (1.f / deltaTime));
        //ImGui::DragFloat("OF", &g_WeatherManager->offsetDegree);
        ImGui::End();

        float* cameraPosition[3];
        cameraPosition[0] = &camera->position.x;
        cameraPosition[1] = &camera->position.y;
        cameraPosition[2] = &camera->position.z;

        ImGui::Begin("Camera");
        ImGui::DragFloat3("Position", *cameraPosition);
        ImGui::Checkbox("Player Cam", &camera->usePlayerCamera);
        ImGui::DragFloat("FOV", &camera->FOV);
        ImGui::DragFloat("Distance", &camera->PLY_DISTANCE);
        ImGui::DragFloat("Angle", &camera->PLY_ANGLE);
        ImGui::End();

        float* position[3];
        position[0] = &lightManager->lights[0].position.x;
        position[1] = &lightManager->lights[0].position.y;
        position[2] = &lightManager->lights[0].position.z;

        float* colors[3];
        colors[0] = &lightManager->lights[0].diffuse.r;
        colors[1] = &lightManager->lights[0].diffuse.g;
        colors[2] = &lightManager->lights[0].diffuse.b;

        ImGui::Begin("Lights");
        ImGui::ColorEdit3("Color", *colors);
        ImGui::DragFloat3("Position", *position);
        //ImGui::DragFloat("Threshold", &waterThreshold, 0.05f, 0.f, 1.f);
        //ImGui::Checkbox("Day & Night cycle", &dayNightCycleOn);
        //ImGui::DragFloat("Cycle speed", &dayNightCycle->speed);
        ImGui::Image((void*)(intptr_t)renderManager->GetDepthMapId(), ImVec2(200, 200));
        ImGui::End();

        float* fogColor[3];
        fogColor[0] = &weatherManager->fogColor.r;
        fogColor[1] = &weatherManager->fogColor.g;
        fogColor[2] = &weatherManager->fogColor.b;

        ImGui::Begin("Fog");
        ImGui::ColorEdit3("Color", *fogColor);
        ImGui::DragFloat("Density", &weatherManager->fogDensity, 0.005f);
        ImGui::DragFloat("Gradient", &weatherManager->fogGradient, 0.03f);
        if (ImGui::Button("Change weather"))
        {
            weatherManager->SetWeather(HAIL);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    Shutdown();

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cCamera::GetInstance()->MoveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cCamera::GetInstance()->MoveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cCamera::GetInstance()->MoveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cCamera::GetInstance()->MoveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        cCamera::GetInstance()->MoveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        cCamera::GetInstance()->MoveDown(deltaTime);

    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //    playerChar->Run(UP);
    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //    playerChar->Run(DOWN);
    //if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //    playerChar->Run(LEFT);
    //if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    //    playerChar->Run(RIGHT);
    //
    //if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    //    playerChar->Walk(UP);
    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    //    playerChar->Walk(DOWN);
    //if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //    playerChar->Walk(LEFT);
    //if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //    playerChar->Walk(RIGHT);

    bool playerDesiresMovement = false;
    eDirection playerDesiredDirection = eDirection::UP;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        playerDesiresMovement = true;
        playerDesiredDirection = eDirection::UP;
    }
    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        playerDesiresMovement = true;
        playerDesiredDirection = eDirection::DOWN;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        playerDesiresMovement = true;
        playerDesiredDirection = eDirection::LEFT;
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        playerDesiresMovement = true;
        playerDesiredDirection = eDirection::RIGHT;
    }

    if (playerDesiresMovement)
    {
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) playerChar->Run(playerDesiredDirection);
        else playerChar->Walk(playerDesiredDirection);
    }
    else
    {
        playerChar->StopMovement();
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // menuing and give/take movement control
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    //glViewport(0, 0, width, height);

    cCamera::GetInstance()->SCR_WIDTH = width;
    cCamera::GetInstance()->SCR_HEIGHT = height;
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        cCamera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}