#include "Engine.h"

#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include "cCamera.h"
#include "cTextureManager.h"
#include "cLightManager.h"
#include "cAnimationManager.h"
#include "cRenderManager.h"
#include "cMapManager.h"
#include "cWeatherManager.h"
#include "cParticleManager.h"
#include "cUIManager.h"

#include "cPlayerCharacter.h"

GLFWwindow* window;
float deltaTime = 0.f;
float lastFrame = 0.f;

bool isFullscreen = false;

const char* resolutions[] = {
    "2560x1400",
    "1920x1080",
    "1600x900",
    "1280x720",
    "900x900",
};

void SetWindowResolution(int width, int height)
{
    cCamera::GetInstance()->SCR_WIDTH = width;
    cCamera::GetInstance()->SCR_HEIGHT = height;
    glfwSetWindowSize(window, width, height);
}

void InitializeImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.WantCaptureMouse = false;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void RenderImgui()
{
    // Imgui
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::ShowDemoWindow();
    
    static int currentSelectedResolutionIndex = 3;

    ImGui::Begin("General");
    ImGui::Text("FPS: %f", (1.f / deltaTime));
    if (ImGui::Button(isFullscreen ? "Window" : "Fullscreen"))
    {
        if (isFullscreen) // set windowed
        {
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
            glfwSetWindowPos(window, mode->width/7, mode->height/7);

            std::string resolution = resolutions[currentSelectedResolutionIndex];
            int pos = resolution.find('x');
            int width = stoi(resolution.substr(0, pos));
            int height = stoi(resolution.substr(pos + 1));
            SetWindowResolution(width, height);
        }
        else // set borlerless window (fullscreen)
        {
            glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
            glfwSetWindowPos(window, 0, 0);
            GLFWmonitor* monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            glfwSetWindowSize(window, mode->width, mode->height);
        }
        
        isFullscreen = !isFullscreen;
    }
    
    std::string resolutionPreviewValue = std::to_string(cCamera::GetInstance()->SCR_WIDTH) + "x" + std::to_string(cCamera::GetInstance()->SCR_HEIGHT); // Pass in the preview value visible before opening the combo (it could be anything)
    float aspectRatio = round(((float)cCamera::GetInstance()->SCR_WIDTH / (float)cCamera::GetInstance()->SCR_HEIGHT) * 100.f) / 100.f;
    if (ImGui::BeginCombo(std::to_string(aspectRatio).c_str(), resolutionPreviewValue.c_str()))
    {
        for (int n = 0; n < IM_ARRAYSIZE(resolutions); n++)
        {
            const bool is_selected = (currentSelectedResolutionIndex == n);
            if (ImGui::Selectable(resolutions[n], is_selected) && !isFullscreen)
            {
                currentSelectedResolutionIndex = n;
                std::string newResolution = resolutions[n];
                int pos = newResolution.find('x');
                int width = stoi(newResolution.substr(0, pos));
                int height = stoi(newResolution.substr(pos + 1));

                SetWindowResolution(width, height);
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::End();

    cLightManager* lightManager = cLightManager::GetInstance();
    cWeatherManager* weatherManager = cWeatherManager::GetInstance();
    cRenderManager* renderManager = cRenderManager::GetInstance();
    cCamera* camera = cCamera::GetInstance();

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

    float* fogColor[3];
    fogColor[0] = &weatherManager->fogColor.r;
    fogColor[1] = &weatherManager->fogColor.g;
    fogColor[2] = &weatherManager->fogColor.b;

    int* shadowSmooth = &lightManager->shadowSampleRadius;

    ImGui::Begin("Envoirnment");
    ImGui::BeginTabBar("Tabs");
    if (ImGui::BeginTabItem("Light"))
    {
        ImGui::ColorEdit3("Color", *colors);
        ImGui::DragFloat3("Position", *position);
        ImGui::DragInt("Smoothing", shadowSmooth);
        //ImGui::DragFloat("Threshold", &waterThreshold, 0.05f, 0.f, 1.f);
        //ImGui::Checkbox("Day & Night cycle", &dayNightCycleOn);
        //ImGui::DragFloat("Cycle speed", &dayNightCycle->speed);
        ImGui::Image((void*)(intptr_t)renderManager->GetDepthMapId(), ImVec2(200, 200));
        ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Fog"))
    {
        ImGui::ColorEdit3("Color", *fogColor);
        ImGui::DragFloat("Density", &weatherManager->fogDensity, 0.005f);
        ImGui::DragFloat("Gradient", &weatherManager->fogGradient, 0.03f);
        if (ImGui::Button("Change weather"))
        {
            weatherManager->SetWeather(HAIL);
        }
        ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ShutdownImgui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

namespace Engine
{
    cPlayerCharacter* playerChar;

    // camera
    float lastX = 1200 / 2.0f;
    float lastY = 640 / 2.0f;
    bool firstMouse = true;

	bool InitializeGLFW()
	{
        // glfw: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // glfw window creation
        window = glfwCreateWindow(1280, 720, "Magik", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
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
            return false;
        }

        return true;
	}

    void StartUpManagers()
    {
        cCamera::GetInstance();

        cTextureManager::GetInstance();

        cLightManager::GetInstance();

        cAnimationManager::GetInstance();

        cRenderManager::GetInstance();

        cMapManager::GetInstance();

        cWeatherManager::GetInstance();

        cParticleManager::GetInstance();

        cUIManager::GetInstance();
    }

    void ShutdownManagers()
    {
        cCamera::DestroyInstance();

        cTextureManager::DestroyInstance();

        cLightManager::DestroyInstance();

        cAnimationManager::DestroyInstance();

        cRenderManager::DestroyInstance();

        cMapManager::DestroyInstance();

        cWeatherManager::DestroyInstance();

        cParticleManager::DestroyInstance();

        cUIManager::DestroyInstance();
    }

    void GameLoop(bool renderDebugInfo)
    {
        if (renderDebugInfo) InitializeImgui();

        float noiseTimer = 0.f;
        float waterThreshold = 0.7;

        while (!glfwWindowShouldClose(window))
        {
            // per-frame time logic
            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            // input
            processInput(window);

            cLightManager* lightManager = cLightManager::GetInstance();
            lightManager->lights[0].direction = -(lightManager->lights[0].position);

            cAnimationManager::GetInstance()->Process(deltaTime);

            cParticleManager::GetInstance()->UpdateSpawners(deltaTime);

            cRenderManager::GetInstance()->DrawScene();

            if (renderDebugInfo) RenderImgui();

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        if (renderDebugInfo) ShutdownImgui();
    }

    void ShutdownGLFW()
    {
        //glfw: terminate, clearing all previously allocated GLFW resources.
        glfwTerminate();
    }

    // process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
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
            if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
                playerChar->Move(playerDesiredDirection, true);
            else
                playerChar->Move(playerDesiredDirection, false);
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
    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        // make sure the viewport matches the new window dimensions; note that width and 
        // height will be significantly larger than specified on retina displays.
        //glViewport(0, 0, width, height);

        cCamera::GetInstance()->SCR_WIDTH = width;
        cCamera::GetInstance()->SCR_HEIGHT = height;
    }


    // glfw: whenever the mouse moves, this callback is called
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
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        //camera.ProcessMouseScroll(static_cast<float>(yoffset));
    }
}