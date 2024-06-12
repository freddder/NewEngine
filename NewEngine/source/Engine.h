#pragma once

struct GLFWwindow;
class cUIStaticImage;

class cCameraManager;
class cLightManager;
class cAnimationManager;
class cRenderManager;
class cMapManager;
class cSceneManager;
class cUIManager;

namespace Manager
{
	extern cCameraManager camera;
	extern cLightManager light;
	extern cAnimationManager animation;
	extern cRenderManager render;
	extern cMapManager map;
	extern cSceneManager scene;
	extern cUIManager ui;
}

namespace Engine
{
	// TEMPORARY
	extern cUIStaticImage* button;
	extern cUIStaticImage* sprite;

	bool InitializeGLFW();

	void StartUpManagers();
	void ShutdownManagers();

	void GameLoop(bool renderDebugInfo);

	void ShutdownGLFW();

	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	void processInput(GLFWwindow* window);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
}