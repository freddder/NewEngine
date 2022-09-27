#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "cRenderManager.h"
#include "Global.h"

cRenderManager::cRenderManager()
{
    g_Camera->SCR_WIDTH = 1200;
    g_Camera->SCR_HEIGHT = 640;

    //******* Create origin offset buffer for non instanced objects *************
    glm::vec4 originOffset = glm::vec4(0.f);

    glGenBuffers(1, &notInstancedOffsetBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, notInstancedOffsetBufferId);

    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glm::vec4),
        (GLvoid*)&originOffset,
        GL_STATIC_DRAW);

    //GLint offset_location = glGetAttribLocation(g_ShaderManager->GetCurrentShaderId(), "oOffset");	    // program
    //glEnableVertexAttribArray(offset_location);
    //glVertexAttribPointer(offset_location, 4,
    //    GL_FLOAT, GL_FALSE,
    //    sizeof(glm::vec4),
    //    (void*)0);
    //glVertexAttribDivisor(offset_location, 1);

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
    cubemapTextureID = g_TextureManager->CreateCubemap(faces);

    //********************** Setup depth map FBO ********************************

    glGenFramebuffers(1, &depthMapFBO);

    glGenTextures(1, &depthMapID);
    glBindTexture(GL_TEXTURE_2D, depthMapID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // setup matrices uniform block
    glGenBuffers(1, &uboMatricesID);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4) + sizeof(int), NULL, GL_DYNAMIC_DRAW); // maybe change GL_STATIC_DRAW to dynamic
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatricesID, 0, 3 * sizeof(glm::mat4) + sizeof(int));

    // setup fog uniform block
    glGenBuffers(1, &uboFogID);

    glBindBuffer(GL_UNIFORM_BUFFER, uboFogID);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4) + 2 * sizeof(float), NULL, GL_DYNAMIC_DRAW); // maybe change GL_STATIC_DRAW to dynamic
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 2, uboFogID, 0, 2 * sizeof(glm::vec4) + 2 * sizeof(float));
}

cRenderManager::~cRenderManager()
{
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &uboMatricesID);
    glDeleteBuffers(1, &uboFogID);
    glDeleteBuffers(1, &notInstancedOffsetBufferId);
}

void cRenderManager::CreateShadderProgram(std::string programName, const char* vertexPath, const char* fragmentPath)
{
    if (programMap.count(programName) != 0) // it exists
    {
        std::cout << "This shadder program already exists" << std::endl;
        return;
    }

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(SHADER_PATH + vertexPath);
        fShaderFile.open(SHADER_PATH + fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. compile shaders
    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    // shader Program
    unsigned int ID;
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    // delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    sShaderProgram newShader;
    newShader.ID = ID;

    programMap.insert(std::pair<std::string, sShaderProgram>(programName, newShader));

    // add Matrices block to matrices
    unsigned int ubMatricesIndex = glGetUniformBlockIndex(newShader.ID, "Matrices");
    glUniformBlockBinding(newShader.ID, ubMatricesIndex, 0);

    // add Lights block to matrices
    g_LightManager->AddProgramToBlock(newShader.ID);

    // add Fog block to matrices
    unsigned int ubFogIndex = glGetUniformBlockIndex(newShader.ID, "Fog");
    glUniformBlockBinding(newShader.ID, ubFogIndex, 2);
}

unsigned int cRenderManager::GetCurrentShaderId()
{
    return programMap[currShader].ID;
}

unsigned int cRenderManager::GetDepthMapId()
{
    return depthMapID;
}

void cRenderManager::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------------------- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n ------------------------------------------------------- " << std::endl;
        }
    }
}

void cRenderManager::use(std::string programName)
{
    if (programMap.count(programName) == 0) // doesnt exists
        return;

    currShader = programName;
    glUseProgram(programMap[currShader].ID);
}

void cRenderManager::setBool(const std::string& name, bool value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], (int)value);
}

void cRenderManager::setInt(const std::string& name, int value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1i(programMap[currShader].uniformLocations[name], value);
}

void cRenderManager::setFloat(const std::string& name, float value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform1f(programMap[currShader].uniformLocations[name], value);
}

void cRenderManager::setMat4(const std::string& name, const glm::mat4& mat)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniformMatrix4fv(programMap[currShader].uniformLocations[name], 1, GL_FALSE, &mat[0][0]);
}

void cRenderManager::setVec2(const std::string& name, const glm::vec2& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform2fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec3(const std::string& name, const glm::vec3& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform3fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::setVec4(const std::string& name, const glm::vec4& value)
{
    if (programMap[currShader].uniformLocations.count(name) == 0)
    {
        unsigned int newLocation = glGetUniformLocation(programMap.at(currShader).ID, name.c_str());
        programMap[currShader].uniformLocations.insert(std::pair<std::string, unsigned int>(name, newLocation));
    }

    glUniform4fv(programMap[currShader].uniformLocations[name], 1, &value[0]);
}

void cRenderManager::AddModel(cRenderModel* newModel)
{
    models.insert(newModel);
}

void cRenderManager::RemoveModel(cRenderModel* model)
{
    models.erase(model);
}

void cRenderManager::DrawObject(cRenderModel* model)
{
    sModelDrawInfo drawInfo;
    if (!g_ModelManager->FindModelByName(model->meshName, drawInfo))
        return;

    use(model->shaderName);
    
    setVec3("modelPosition", model->position);
    setMat4("modelOrientationX", glm::rotate(glm::mat4(1.0f), model->orientation.x, glm::vec3(1.f, 0.f, 0.f)));
    setMat4("modelOrientationY", glm::rotate(glm::mat4(1.0f), model->orientation.y, glm::vec3(0.f, 1.f, 0.f)));
    setMat4("modelOrientationZ", glm::rotate(glm::mat4(1.0f), model->orientation.z, glm::vec3(0.f, 0.f, 1.f)));
    setMat4("modelScale", glm::scale(glm::mat4(1.0f), model->scale));

    setBool("useWholeColor", model->useWholeColor);
    setVec4("wholeColor", model->wholeColor);

    model->SetUpUniforms();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMapID);
    setInt("shadowMap", 1);

    for (unsigned int i = 0; i < drawInfo.allMeshesData.size(); i++)
    {
        // Setup texture
        std::string textureToUse;

        if (model->textureName == "")
            textureToUse = drawInfo.allMeshesData[i].textureName;
        else
            textureToUse = model->textureName;

        g_TextureManager->SetupTexture(textureToUse, 0);

        // Bind VAO
        glBindVertexArray(drawInfo.allMeshesData[i].VAO_ID);

        // Check for instanced
        if (model->isInstanced)
        {
            glBindBuffer(GL_ARRAY_BUFFER, model->instanceOffsetsBufferId);

            GLint offset_location = glGetAttribLocation(programMap[currShader].ID, "oOffset");
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

            GLint offset_location = glGetAttribLocation(programMap[currShader].ID, "oOffset");
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

void cRenderManager::DrawScene()
{
    //********************** Shadow pass ********************************

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);

    
    glm::mat4 lightProjection, lightView;
    float near_plane = 1.f, far_plane = 100.f;

    glm::vec3 lightPos = glm::vec3(g_LightManager->lights[0].position);// + pSprite->positionXYZ;
    glm::vec3 lightAt = glm::vec3(0.f, 0.f, 0.f);// pSprite->positionXYZ;

    lightProjection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, lightAt, glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = lightProjection * lightView;

    int bruh = 1; // B R U H

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(lightProjection));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightView));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(int), &bruh);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);


    //Draw scene
    for (std::set<cRenderModel*>::iterator it = models.begin(); it != models.end(); it++)
    {
        DrawObject(*it);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //*********************** Regular pass ******************************
    // reset viewport
    glViewport(0, 0, g_Camera->SCR_WIDTH, g_Camera->SCR_HEIGHT);
    glClearColor(0.89f, 0.89f, 0.89f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //g_LightManager->SetUnimormValues(programMap[currShader].ID);
    g_LightManager->SetUnimormValues();

    // pass projection matrix to shader (note that in this case it could change every frame)
    glm::mat4 projection = g_Camera->GetProjectionMatrix();

    // camera/view transformation
    glm::mat4 view = g_Camera->GetViewMatrix();

    bruh = 0;

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatricesID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(lightSpaceMatrix));
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * sizeof(glm::mat4), sizeof(int), &bruh);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBuffer(GL_UNIFORM_BUFFER, uboFogID);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(*g_Camera->playerPosition, 1.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * sizeof(glm::vec4), sizeof(glm::vec4), glm::value_ptr(glm::vec4(g_WeatherManager->fogColor, 1.f)));
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(float), &g_WeatherManager->fogDensity);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4) + sizeof(float), sizeof(float), &g_WeatherManager->fogGradient);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Draw scene
    for (std::set<cRenderModel*>::iterator it = models.begin(); it != models.end(); it++)
    {
        DrawObject(*it);
    }

    // Draw weather particles

    use("snow");

    projection = glm::ortho(-25.0f, 25.0f, -25.0f, 25.0f, 1.f, 100.f);
    view = g_Camera->GetViewMatrix();

    setMat4("view", view);
    setMat4("projection", projection);
    setFloat("scrWidth", g_Camera->SCR_WIDTH);
    setFloat("scrHeight", g_Camera->SCR_HEIGHT);
    setFloat("swingDegree", g_WeatherManager->offsetDegree);
    setVec3("cameraPosition", g_Camera->position);

    for (unsigned int i = 0; i < g_WeatherManager->particleTypes.size(); i++)
    {
        g_TextureManager->SetupTexture(g_WeatherManager->particleTypes[i].textureName, 0);

        sModelDrawInfo drawInfo;
        g_ModelManager->FindModelByName(g_WeatherManager->particleTypes[i].modelName, drawInfo);

        glBindVertexArray(drawInfo.allMeshesData[0].VAO_ID);

        glBindBuffer(GL_ARRAY_BUFFER, g_WeatherManager->particleTypes[i].positionsBufferId);

        GLint offset_location = glGetAttribLocation(programMap[currShader].ID, "oOffset");
        glEnableVertexAttribArray(offset_location);
        glVertexAttribPointer(offset_location, 2,
            GL_FLOAT, GL_FALSE,
            sizeof(glm::vec2),
            (void*)0);
        glVertexAttribDivisor(offset_location, 1);

        glDrawElementsInstanced(GL_TRIANGLES,
            drawInfo.allMeshesData[0].numberOfIndices,
            GL_UNSIGNED_INT,
            (void*)0,
            g_WeatherManager->particleTypes[i].positions.size());

        glBindVertexArray(0);
    }

    // Draw skybox
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    use("skybox");
    view = glm::mat4(glm::mat3(g_Camera->GetViewMatrix())); // remove translation from the view matrix
    projection = g_Camera->GetProjectionMatrix();

    setMat4("view", view);
    setMat4("projection", projection);

    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}