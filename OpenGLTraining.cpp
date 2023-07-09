/*
	https://ithelp.ithome.com.tw/articles/10284098
	https://www.glfw.org/documentation.html
	https://www.opengl-tutorial.org/
	https://stackoverflow.com/questions/43162311/visual-studio-community-2017-lnk2019-unresolved-external-symbol
	pre-requirement: GLFW, GLEW, CMake
	Visual Stuidio Project Property Setting: add library, header, linker of GLFW and GLEW
	Remember: add GLEW dll(glew32.dll) in project folder

	GLFW: GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
	GLEW: The OpenGL Extension Wrangler Library (GLEW) is a cross-platform open-source C/C++ extension loading library.
	Training(2023-07-09): OpenGL window, change color by press space
*/


/////OpenGL libraries/////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/////Standard C++ header/////
#include <iostream>
#include <cstdlib>

using namespace std;

// https://learn.microsoft.com/zh-tw/cpp/cpp/data-type-ranges?view=msvc-170
// wish I can remember all types one day
const unsigned short SCREEN_WIDTH = 800;
const unsigned short SCREEN_HEIGHT = 600;

int main(void)
{
	cout << "hello OpenGL" << endl;

	if (!glfwInit()) // Initialize GLFW library. Have to do this first, otherwise APIs won't work.
	{
		cout << "Initializing GLFW library failed." << endl;
		return -1;
	}

	/*
		glfwWindowHint(hint, value): This function sets hints for the next call to glfwCreateWindow(). 
		GLFW_SAMPLES: Framebuffer MSAA samples
		GLFW_CONTEXT_VERSION_MAJOR, GLFW_CONTEXT_VERSION_MINOR: specify the OpenGL API version
		GLFW_OPENGL_PROFILE: specifies which OpenGL profile to create the context for
		GLFW_OPENGL_CORE_PROFILE: Possible value for GLFW_OPENGL_PROFILE. If need older version like OpenGL 3.2, can use GLFW_OPENGL_ANY_PROFILE
	*/
	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef MACOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
#endif // MACOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OpenGL window", NULL, NULL); // Handler of OpenGL window

	if (!window) // window = NULL = false = 0
	{
		cout << "initializing OpenGL window failed." << endl;
		glfwTerminate(); // Terminate the OpenGL window
		return -1;
	}

	/*
		Rendering Contexts: https://learn.microsoft.com/en-us/windows/win32/opengl/rendering-contexts
		An OpenGL rendering context is a port through which all OpenGL commands pass.
		Every thread that makes OpenGL calls must have a current rendering context.
		Rendering contexts link OpenGL to the Windows windowing systems.
	*/
	glfwMakeContextCurrent(window); // make the windows's context.

	if (glewInit() != GLEW_OK)
	{
		cout << "Initializing GLEW failed." << endl;
		return -1;
	}

	/*
		glfwSetInputMode(): This function sets an input mode option for the specified window.
		GLFW_STICKY_KEYS: When sticky keys mode is enabled, 
						  the pollable state of a key will remain GLFW_PRESS until the state of that key is polled with glfwGetKey. 
						  Once it has been polled, if a key release event had been processed in the meantime, 
						  the state will reset to GLFW_RELEASE, otherwise it will remain GLFW_PRESS.
	*/
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	unsigned short space_pressed = 0; // for switch screen color
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) // Press and release Space in the same time
		{
			space_pressed++;
			cout << "space pressed: " << space_pressed << endl;
		}

		switch (space_pressed % 4) // lol...
		{
			case 0:
				glClearColor(0.f, 0.f, 0.f, 1.0f); // RGB
				break;
			case 1:
				glClearColor(1.f, 0.f, 0.f, 1.f);
				break;
			case 2:
				glClearColor(0.f, 1.f, 0.f, 1.f);
				break;
			case 3:
				glClearColor(0.f, 0.f, 1.f, 1.f);
				break;
			default:
				glClearColor(0.f, 0.f, 0.f, 1.0f);
		}
		
		/*
			glClear(): clear buffers to preset values
			GL_COLOR_BUFFER_BIT: constant, tell glClear() which buffer I want to clear.
			https://stackoverflow.com/questions/5479951/what-is-the-purpose-of-gl-color-buffer-bit-and-gl-depth-buffer-bit
		*/
		glClear(GL_COLOR_BUFFER_BIT); // render here
		glfwSwapBuffers(window); // swap front and back buffer

		/*
			This function processes only those events that are already in the event queue and then returns immediately.
			Processing events will cause the window and input callbacks associated with those events to be called.
			On some platforms, a window move, resize or menu operation will cause event processing to block.
		*/
		glfwPollEvents(); // poll and process events
	}

	system("pause");
	return 0;
}

