#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cPlayerCharacter;
class cUIStaticImage;

namespace Engine
{
	// TEMPORARY
	extern cPlayerCharacter* playerChar;
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