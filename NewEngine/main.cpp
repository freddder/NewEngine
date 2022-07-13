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

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void DrawObject(cModel* model);

// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 640;

unsigned int notInstancedOffsetBufferId;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    g_ShaderManager->CreateShadderProgram("scene", "VertShader1.glsl", "FragShader1.glsl");
    g_ShaderManager->CreateShadderProgram("skybox", "SkyboxVertShader.glsl", "SkyboxFragShader.glsl");
    //g_ShaderManager->CreateShadderProgram("sprite", "SpriteVertShader.glsl", "SpriteFragShader.glsl");
    //g_ShaderManager->CreateShadderProgram("depth", "DepthVertShader.glsl", "DepthFragShader.glsl");
    //g_ShaderManager->CreateShadderProgram("debug", "DebugVertShader.glsl", "DebugFragShader.glsl");
    g_ShaderManager->use("scene");

    // configure global opengl state
    glEnable(GL_DEPTH_TEST);

    //********************** Prepare Light **************************************

    g_LightManager->lights[0].extraParam.x = 2.f; // directional light
    g_LightManager->lights[0].position = glm::vec4(-10.f, 10.f, 10.f, 1.f);
    g_LightManager->lights[0].direction = -(g_LightManager->lights[0].position);
    g_LightManager->lights[0].extraParam.w = 1.f; // turn on

    g_ShaderManager->use("scene");
    g_LightManager->SetupUniformLocations(g_ShaderManager->GetCurrentShaderId());

    //********************** Load models ****************************************

    std::vector<std::string> modelsToLoad;
    //modelsToLoad.push_back("Mistralton City House.obj");
    modelsToLoad.push_back("r0_treePine.obj");
    modelsToLoad.push_back("TestMap1.obj");
    modelsToLoad.push_back("ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply");
    modelsToLoad.push_back("SpriteHolder.obj");

    for (unsigned int i = 0; i < modelsToLoad.size(); i++)
        g_ModelManager->LoadModel(modelsToLoad[i], g_ShaderManager->GetCurrentShaderId());

    cModel* debugSphere = new cModel();
    debugSphere->meshName = "ISO_Shphere_flat_4div_xyz_n_rgba_uv.ply";
    debugSphere->position = glm::vec3(0.f, 10.f, 0.f);
    debugSphere->useWholeColor = true;
    debugSphere->wholeColor = glm::vec4(0.f, 1.f, 0.f, 1.f);
    g_vec_pModels.push_back(debugSphere);

    cModel* map = new cModel();
    map->meshName = "TestMap1.obj";
    //map->scale = glm::vec3(0.07f);
    //map->orientation.x = glm::radians(-90.f);
    g_vec_pModels.push_back(map);

    cModel* tree = new cModel();
    tree->meshName = "r0_treePine.obj";
    g_vec_pModels.push_back(tree);

    cModel* sprite = new cModel();
    sprite->meshName = "SpriteHolder.obj";
    sprite->position.z = 10;
    sprite->textureName = "Nate.png";
    g_TextureManager->CreateTexture("Nate.png");
    sprite->isAnimated = true;
    g_vec_pModels.push_back(sprite);

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


    //******* Create origin offset buffer for non instanced objects *************
    glm::vec4 originOffset = glm::vec4(0.f);

    glGenBuffers(1, &notInstancedOffsetBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, notInstancedOffsetBufferId);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glm::vec4),
        (GLvoid*)&originOffset,
        GL_STATIC_DRAW);

    GLint offset_location = glGetAttribLocation(g_ShaderManager->GetCurrentShaderId(), "oOffset");	    // program
    glEnableVertexAttribArray(offset_location);
    glVertexAttribPointer(offset_location, 4,
        GL_FLOAT, GL_FALSE,
        sizeof(glm::vec4),   
        (void*)0);
    glVertexAttribDivisor(offset_location, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //*************** Setup skybox vertices and VAOs ***************************

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load skybox textures
    std::vector<std::string> faces
    {
        "TropicalSunnyDayRight.jpg",    //"right.jpg",
        "TropicalSunnyDayLeft.jpg",     //"left.jpg",
        "TropicalSunnyDayUp.jpg",      //"top.jpg",
        "TropicalSunnyDayDown.jpg",   //"bottom.jpg",
        "TropicalSunnyDayFront.jpg",    //"front.jpg",
        "TropicalSunnyDayBack.jpg",     //"back.jpg"
    };
    unsigned int cubemapTexture = g_TextureManager->CreateCubemap(faces);

    //********************** Setup depth map FBO ********************************

    const unsigned int SHADOW_WIDTH = 3048, SHADOW_HEIGHT = 3048;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);

    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    g_ShaderManager->use("scene");
    g_ShaderManager->setInt("shadowMap", 1);

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

    //******************** Animation testing ************************************



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

        //********************** Shadow pass ********************************

        g_ShaderManager->use("scene");

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        float near_plane = 1.f, far_plane = 100.f;

        glm::vec3 lightPos = glm::vec3(g_LightManager->lights[0].position);// + pSprite->positionXYZ;
        glm::vec3 lightAt = glm::vec3(0.f, 0.f, 0.f);// pSprite->positionXYZ;
        
        //lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
        lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, lightAt, glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        //g_ShaderManager->use("depth");
        g_ShaderManager->setMat4("lightSpaceMatrix", lightSpaceMatrix);

        g_ShaderManager->setBool("isShadowPass", true);

        //Draw scene
        for (unsigned int i = 0; i < g_vec_pModels.size(); i++)
        {
            DrawObject(g_vec_pModels[i]);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //*********************** Regular pass ******************************

        // reset viewport
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.f, 0.8f, 1.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_LightManager->SetUnimormValues(g_ShaderManager->GetCurrentShaderId());

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(g_Camera->FOV, (float)SCR_WIDTH / (float)SCR_HEIGHT, g_Camera->nearPlane, g_Camera->farPlane);
        g_ShaderManager->setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = g_Camera->GetViewMatrix();
        g_ShaderManager->setMat4("view", view);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        //g_ShaderManager->setMat4("lightSpaceMatrix", lightSpaceMatrix);
        g_ShaderManager->setBool("isShadowPass", false);

        // Draw scene
        for (unsigned int i = 0; i < g_vec_pModels.size(); i++)
        {
            DrawObject(g_vec_pModels[i]);
        }

        //debugSphere->position.y += 10.f;

        // Draw skybox
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        g_ShaderManager->use("skybox");
        view = glm::mat4(glm::mat3(g_Camera->GetViewMatrix())); // remove translation from the view matrix
        g_ShaderManager->setMat4("view", view);
        g_ShaderManager->setMat4("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        //****************** Render debug quad ******************************

        //g_ShaderManager->use("debug");
        //g_ShaderManager->setFloat("near_plane", near_plane);
        //g_ShaderManager->setFloat("far_plane", far_plane);
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, depthMap);

        //glBindVertexArray(quadVAO);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        //glBindVertexArray(0);

        //*******************************************************************

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
        ImGui::Image((void*)(intptr_t)depthMap, ImVec2(200, 200));
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

    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
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

void DrawObject(cModel* model)
{
    sModelDrawInfo drawInfo;
    if (g_ModelManager->FindModelByName(model->meshName, drawInfo))
    {
        g_ShaderManager->setVec3("modelPosition", model->position);
        g_ShaderManager->setMat4("modelOrientationX", glm::rotate(glm::mat4(1.0f), model->orientation.x, glm::vec3(1.f, 0.f, 0.f)));
        g_ShaderManager->setMat4("modelOrientationY", glm::rotate(glm::mat4(1.0f), model->orientation.y, glm::vec3(0.f, 1.f, 0.f)));
        g_ShaderManager->setMat4("modelOrientationZ", glm::rotate(glm::mat4(1.0f), model->orientation.z, glm::vec3(0.f, 0.f, 1.f)));
        g_ShaderManager->setMat4("modelScale", glm::scale(glm::mat4(1.0f), model->scale));

        g_ShaderManager->setBool("useWholeColor", model->useWholeColor);
        g_ShaderManager->setVec4("wholeColor", model->wholeColor);

        g_ShaderManager->setBool("isSpriteAnimated", model->isAnimated);

        g_ShaderManager->setInt("numCols", 3);
        g_ShaderManager->setInt("numRows", 8);
        g_ShaderManager->setInt("spriteId", 0);

        for (unsigned int i = 0; i < drawInfo.allMeshesData.size(); i++)
        {
            // Setup texture
            std::string textureToUse;

            if (model->textureName == "")
                textureToUse = drawInfo.allMeshesData[i].textureName;
            else
                textureToUse = model->textureName;
            
            g_TextureManager->SetupTexture(textureToUse);

            // Bind VAO
            glBindVertexArray(drawInfo.allMeshesData[i].VAO_ID);

            // Check for instanced
            if (model->isInstanced)
            {
                glBindBuffer(GL_ARRAY_BUFFER, model->instanceOffsetsBufferId);

                GLint offset_location = glGetAttribLocation(g_ShaderManager->GetCurrentShaderId(), "oOffset");
                glEnableVertexAttribArray(offset_location);
                glVertexAttribPointer(offset_location, 4,
                    GL_FLOAT, GL_FALSE,
                    sizeof(glm::vec4),
                    (void*)0);
                glVertexAttribDivisor(offset_location, 1);

                glDrawElementsInstanced(GL_TRIANGLES,
                    drawInfo.allMeshesData[i].numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0,
                    model->instancedNum);
            }
            else
            {
                glBindBuffer(GL_ARRAY_BUFFER, notInstancedOffsetBufferId);

                GLint offset_location = glGetAttribLocation(g_ShaderManager->GetCurrentShaderId(), "oOffset");
                glEnableVertexAttribArray(offset_location);
                glVertexAttribPointer(offset_location, 4,
                    GL_FLOAT, GL_FALSE,
                    sizeof(glm::vec4),
                    (void*)0);
                glVertexAttribDivisor(offset_location, 1);

                glDrawElements(GL_TRIANGLES,
                    drawInfo.allMeshesData[i].numberOfIndices,
                    GL_UNSIGNED_INT,
                    (void*)0);
            }

            glBindVertexArray(0);
        }
    }
}