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
#include "cModelAnimation.h"
#include "cCharacter.h"
#include "cSpriteModel.h"

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

cCharacter* asymCharacter;

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

    // Setup shader programs
    g_RenderManager->CreateShadderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    g_RenderManager->CreateShadderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    g_RenderManager->CreateShadderProgram("sprite", "SpriteVertShader.glsl", "SpriteFragShader.glsl");
    g_RenderManager->CreateShadderProgram("wave", "WaveVertShader.glsl", "WaveFragShader.glsl");
    g_RenderManager->CreateShadderProgram("ocean", "OceanVertShader.glsl", "OceanFragShader.glsl");
    g_RenderManager->CreateShadderProgram("snow", "SnowVertShader.glsl", "SnowFragShader.glsl");
    //g_RenderManager->CreateShadderProgram("debug", "DebugVertShader.glsl", "DebugFragShader.glsl");

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);


    //********************** Prepare Light **************************************

    g_LightManager->lights[0].extraParam.x = 2.f; // directional light
    g_LightManager->lights[0].position = glm::vec4(-10.f, 10.f, 10.f, 1.f);
    g_LightManager->lights[0].direction = -(g_LightManager->lights[0].position);
    g_LightManager->lights[0].extraParam.w = 1.f; // turn on


    //********************** Load models and textures ***************************

    g_ModelManager->LoadModel("r0_treePine.obj", "scene");
    g_ModelManager->LoadModel("TestMapWater.obj", "scene");
    g_ModelManager->LoadModel("SpriteHolder.obj", "sprite");
    g_ModelManager->LoadModel("SpriteHolder2.obj", "snow");
    g_ModelManager->LoadModel("Water_c2.obj", "wave");
    g_ModelManager->LoadModel("Water_b2.obj", "wave");
    g_ModelManager->LoadModel("Water_bl2.obj", "wave");
    g_ModelManager->LoadModel("sea_water2.obj", "ocean");

    g_TextureManager->CreateSpriteSheet("Nate.png", 3, 8, false);
    g_TextureManager->CreateSpriteSheet("SymetricNPC_1.png", 2, 4, true);
    g_TextureManager->CreateSpriteSheet("AsymetricalNPC_1.png", 3, 4, false);

    g_TextureManager->CreateTexture("SnowFlake1.png");
    g_TextureManager->CreateTexture("SnowFlake2.png");
    g_TextureManager->CreateTexture("SnowFlake3.png");

    cRenderModel* tree = new cRenderModel();
    tree->meshName = "r0_treePine.obj";
    //g_set_Models.insert(tree);
    g_RenderManager->AddModel(tree);

    cSpriteModel* sprite = new cSpriteModel();
    sprite->meshName = "SpriteHolder.obj";
    sprite->textureName = "Nate.png";
    sprite->currSpriteId = 3;
    //g_set_Models.insert(sprite);
    g_RenderManager->AddModel(sprite);

    cSpriteAnimation* spriteAnimation = new cSpriteAnimation(sprite->currSpriteId, sprite->scale);
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 4, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.4f, 3, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.6f, 5, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(0.8f, 3, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(1.0f, 1, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(1.2f, 0, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(1.4f, 2, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(1.6f, 0, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(1.8f, 7, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(2.0f, 6, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(2.2f, 8, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(2.4f, 6, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(2.6f, 10, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(2.8f, 9, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(3.0f, 11, false));
    spriteAnimation->AddKeyFrame(sKeyFrameSprite(3.2f, 9, false));

    spriteAnimation->isRepeat = true;
    g_AnimationManager->AddAnimation(spriteAnimation);    

    asymCharacter = new cCharacter(glm::vec3(0.f, 0.f, 2.f), "SymetricNPC_1.png");

    g_MapManager->LoadMap("", "");

    cSpriteModel* spriteSym = new cSpriteModel();
    spriteSym->meshName = "SpriteHolder.obj";
    spriteSym->position.z = -2.f;
    spriteSym->textureName = "SymetricNPC_1.png";
    spriteSym->currSpriteId = 2;
    g_RenderManager->AddModel(spriteSym);

    cSpriteAnimation* symSpriteAnimation = new cSpriteAnimation(spriteSym->currSpriteId, spriteSym->scale);
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.2f, 3, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.4f, 2, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.6f, 3, true));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(0.8f, 2, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.0f, 1, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.2f, 0, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.4f, 1, true));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.6f, 0, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(1.8f, 5, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.0f, 4, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.2f, 6, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.4f, 4, false));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.6f, 5, true));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(2.8f, 4, true));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(3.0f, 6, true));
    symSpriteAnimation->AddKeyFrame(sKeyFrameSprite(3.2f, 4, true));
    symSpriteAnimation->isRepeat = true;
    g_AnimationManager->AddAnimation(symSpriteAnimation);

    cRenderModel* snow = new cRenderModel();
    //snow->position.x = -1.5f;
    snow->meshName = "SpriteHolder2.obj";
    snow->textureName = "SnowFlake1.png";
    snow->shaderName = "snow";
    //g_RenderManager->AddModel(snow);

    cRenderModel* snow2 = new cRenderModel();
    snow2->position.x = -0.5f;
    snow2->meshName = "SpriteHolder2.obj";
    snow2->textureName = "SnowFlake2.png";
    snow2->shaderName = "snow";
    //g_RenderManager->AddModel(snow);

    cRenderModel* snow3 = new cRenderModel();
    snow3->position.x = 0.5f;
    snow3->meshName = "SpriteHolder2.obj";
    snow3->textureName = "SnowFlake3.png";
    snow3->shaderName = "snow";
    //g_RenderManager->AddModel(snow);

    //cModel* house = new cModel();
    //house->meshName = "Mistralton City House.obj";
    //house->scale = glm::vec3(0.07f);
    //g_vec_pModels.push_back(house);

    //std::vector<glm::vec4> offsets;
    //for (unsigned int i = 0; i < 100000; i++)
    //{
    //    offsets.push_back(glm::vec4(i));
    //}
    //tree->InstanceObject(offsets, g_ShaderManager->GetCurrentShaderId());

    //********************** Setup on screen texture ****************************

    //unsigned int quadVAO;
    //unsigned int quadVBO;

    //float quadVertices[] = {
    //    // positions        // texture Coords
    //    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    //    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    //     1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
    //     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    //};

    //// setup plane VAO
    //glGenVertexArrays(1, &quadVAO);
    //glGenBuffers(1, &quadVBO);
    //glBindVertexArray(quadVAO);
    //glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    //glEnableVertexAttribArray(0);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(1);
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

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

        g_LightManager->lights[0].direction = -(g_LightManager->lights[0].position);

        g_AnimationManager->Update(deltaTime);

        g_RenderManager->DrawScene();

        g_RenderManager->use("snow");

        glm::mat4 proj = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 1.f, 100.f);
        glm::mat4 view = g_Camera->GetViewMatrix();

        //g_RenderManager->setMat4("projection", proj);
        //g_RenderManager->setMat4("view", view);

        //snow->position.x += deltaTime * 100;

        g_RenderManager->setFloat("scrWidth", g_Camera->SCR_WIDTH);
        g_RenderManager->setFloat("scrHeight", g_Camera->SCR_HEIGHT);

        g_RenderManager->DrawObject(snow);
        g_RenderManager->DrawObject(snow2);
        g_RenderManager->DrawObject(snow3);

        // Imgui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("General");
        ImGui::Text("FPS: %f", (1.f / deltaTime));
        ImGui::End();

        float* position[3];
        position[0] = &g_LightManager->lights[0].position.x;
        position[1] = &g_LightManager->lights[0].position.y;
        position[2] = &g_LightManager->lights[0].position.z;

        float* colors[3];
        colors[0] = &g_LightManager->lights[0].diffuse.r;
        colors[1] = &g_LightManager->lights[0].diffuse.g;
        colors[2] = &g_LightManager->lights[0].diffuse.b;

        ImGui::Begin("Lights");
        ImGui::ColorEdit3("Color", *colors);
        ImGui::DragFloat3("Position", *position);
        //ImGui::Checkbox("Day & Night cycle", &dayNightCycleOn);
        //ImGui::DragFloat("Cycle speed", &dayNightCycle->speed);
        ImGui::Image((void*)(intptr_t)g_RenderManager->GetDepthMapId(), ImVec2(200, 200));
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
        g_Camera->MoveForward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        g_Camera->MoveBackward(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        g_Camera->MoveLeft(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        g_Camera->MoveRight(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        g_Camera->MoveUp(deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        g_Camera->MoveDown(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        asymCharacter->Walk(UP);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        asymCharacter->Walk(DOWN);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        asymCharacter->Walk(LEFT);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        asymCharacter->Walk(RIGHT);
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

    g_Camera->SCR_WIDTH = width;
    g_Camera->SCR_HEIGHT = height;
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

        g_Camera->ProcessMouseMovement(xoffset, yoffset);
    }
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}