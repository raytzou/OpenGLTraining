/*
	https://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/
	pre-requirement: GLM (OpenGL Mathematics)

	Training(2023-07-16): Matrices (OpenGL Mathematics)
*/


/////OpenGL libraries/////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/////OpenGL Mathematics/////
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp> // for printing vector
/////Standard C++ header/////
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace glm;

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

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef MACOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
#endif // MACOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OpenGL window", NULL, NULL); // Handler of OpenGL window

	if (!window)
	{
		cout << "initializing OpenGL window failed." << endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); // make the windows's context.

	if (glewInit() != GLEW_OK)
	{
		cout << "Initializing GLEW failed." << endl;
		return -1;
	}

	mat4 matrix = translate(mat4(1.0f), vec3(10.0f, .0f, .0f)); // 4x4 matrix
	vec4 vector(10.0f, 10.0f, 10.0f, 1.0f);
	/*
		matrix = [
					1, 0, 0, 10
					0, 1, 0, 0
					0, 0, 1, 0
					0, 0, 0, 1
					]
		vector = (10, 10, 10, 1)
	*/
	vec4 transformVector = matrix * vector;
	/*
		after transform, in Mathematic, it means we get a Homogeneous Vecor and it represents a position but not direction
		transformVecot = (20, 10, 10, 1) 
	*/

	cout << "matrix: " << to_string(matrix) << endl;
	cout << "vector: " << to_string(vector) << endl;
	cout << "transformVector: " << to_string(transformVector) << endl;

	mat4 scalingMatrix = scale(vec3(2.0f, 2.0f, 2.0f));
	cout << "scaling transformVector: " << to_string(scalingMatrix * transformVector) << endl;

	vec3 rotationAxis(1.0, .0f, .0f);
	mat4 rotationMatrix = rotate(90.0f, rotationAxis); // roatate x axis in 90 degrees, and it seems becoming a matrix
	cout << "rotate: " << to_string(rotationMatrix * transformVector) << endl; // (20.000000, -13.420702, 4.459230, 1.000000), what?

	vec4 finalTransformedVector = transformVector * rotationMatrix * scalingMatrix; // !!IMPORTANT!! remember do rotation or other transform before scaling
	cout << "final (rotate, scaling) transform: " << to_string(finalTransformedVector) << endl; // vec4(40.000000, 8.918460, -26.841404, 1.000000)

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	/*
		glfwSetInputMode(): This function sets an input mode option for the specified window.
		GLFW_STICKY_KEYS: When sticky keys mode is enabled,
						  the pollable state of a key will remain GLFW_PRESS until the state of that key is polled with glfwGetKey.
						  Once it has been polled, if a key release event had been processed in the meantime,
						  the state will reset to GLFW_RELEASE, otherwise it will remain GLFW_PRESS.
	*/
	glClearColor(.0f, .0f, .0f, .0f); // Not like glClear(), it doesn't clear the buffer. More like set color. glClearColor() is State Setting

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT); // render here
		/*
			glClear(): clear buffers to preset values
			GL_COLOR_BUFFER_BIT: constant, tell glClear() which buffer I want to clear.
			https://stackoverflow.com/questions/5479951/what-is-the-purpose-of-gl-color-buffer-bit-and-gl-depth-buffer-bit
		*/
		glfwSwapBuffers(window); // swap front and back buffer

		glfwPollEvents(); // poll and process events
		/*
			This function processes only those events that are already in the event queue and then returns immediately.
			Processing events will cause the window and input callbacks associated with those events to be called.
			On some platforms, a window move, resize or menu operation will cause event processing to block.
		*/
	}

	system("pause");
	return 0;
}

