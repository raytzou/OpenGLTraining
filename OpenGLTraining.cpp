/*
	https://ithelp.ithome.com.tw/articles/10284098
	https://www.glfw.org/documentation.html
	pre-requirement: GLFW, GLAD, CMake
	Visual Stuidio Project Property Setting: add GLFW and GLAD library and header

	GLFW: GLFW is an Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan development on the desktop.
	GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.
*/

//#define ITHELP
#define OFFICIAL

#ifdef OFFICIAL
	/////GLFW and GLAD/////
	//#include <glad/glad.h>
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
		glfwInit(); // Initialize GLFW library. Have to do this first, otherwise APIs won't work.
		GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OpenGL window", NULL, NULL); // Handler of window

		if (!window) // window = NULL = false = 0
		{
			cout << "OpenGL init failed" << endl;
			glfwTerminate(); // Terminate the OpenGL window
			return -1;
		}

		glfwMakeContextCurrent(window); // make the windows's context.
		// Rendering Contexts: https://learn.microsoft.com/en-us/windows/win32/opengl/rendering-contexts
		/*
			An OpenGL rendering context is a port through which all OpenGL commands pass.
			Every thread that makes OpenGL calls must have a current rendering context.
			Rendering contexts link OpenGL to the Windows windowing systems.
		*/

		while (!glfwWindowShouldClose(window)) // how do I know when to close lol? alt F4?
		{
			glClearColor(0.0f, 0.0f, 1.f, 1.0f); // RGB
			glClear(GL_COLOR_BUFFER_BIT); // render here
			// glClear(): clear buffers to preset values
			// GL_COLOR_BUFFER_BIT: constant, tell glClear() which buffer I want to clear.
			// https://stackoverflow.com/questions/5479951/what-is-the-purpose-of-gl-color-buffer-bit-and-gl-depth-buffer-bit

			glfwSwapBuffers(window); // swap front and back buffer

			glfwPollEvents(); // poll and process events
			/*
				This function processes only those events that are already in the event queue and then returns immediately.
				Processing events will cause the window and input callbacks associated with those events to be called.
				On some platforms, a window move, resize or menu operation will cause event processing to block.
			*/
		}

		cout << "hello OpenGL" << endl;

		system("pause");
		return 0;
	}
#endif // !OFFICIAL

#ifdef ITHELP
	#include <glad/glad.h>
	#include <glfw/glfw3.h>
	#include <iostream>
	// 設置窗口尺寸
	const unsigned int SCR_WIDTH = 400;
	const unsigned int SCR_HEIGHT = 300;

	int main()
	{
		// glfw: 初始化
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		// uncomment this statement to fix compilation on OS X
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
		// glfw 創建窗口
		GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test<GLFW+GLAD>", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		// glad: load all OpenGL function pointers
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		// render loop
		while (!glfwWindowShouldClose(window))
		{
			glClearColor(0.0f, 1.f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			// glfw: swap buffers and poll IO events (keyspressed/released, mouse moved etc.)
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		// glfw: terminate, clearing all previously allocated GLFWresources.
		glfwTerminate();
		return 0;
	}
#endif // ITHELP