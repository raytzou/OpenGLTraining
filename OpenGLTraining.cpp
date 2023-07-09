/*
	https://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
	https://stackoverflow.com/questions/21652546/what-is-the-role-of-glbindvertexarrays-vs-glbindbuffer-and-what-is-their-relatio
	Training(2023-07-10): First triangle, VAO (Vertex Array Object)
*/

#define DEBUG

#ifdef DEBUG
/////OpenGL libraries/////
#include <GL/glew.h>
#include <GLFW/glfw3.h>
/////Standard C++ header/////
#include <iostream>
#include <cstdlib>

#include "OpenGL/common/shader.cpp" // shader from tutorial

using namespace std;

const unsigned short SCREEN_WIDTH = 800;
const unsigned short SCREEN_HEIGHT = 600;

static const GLfloat g_vertex_buffer_data[] = // vertex array, GLfloat = float
{
	-1.f, -1.f, 0.f, // (-1, -1), Z-axis doesn't matter in 2D world
	1.f, -1.f, 0.f, // (1, -1)
	0.f, 1.f, 0.f // (0, 1)
};


int main(void)
{
	cout << "first triangle" << endl;

	if (!glfwInit())
	{
		cout << "Initializing GLFW library failed." << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
#ifdef MACOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif // MACOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My OpenGL window", NULL, NULL); // Handler of OpenGL window
	if (!window)
	{
		cout << "initializing OpenGL window failed." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		cout << "Initializing GLEW failed." << endl;
		glfwTerminate();
		return -1;
	}

	// VAO should be created after initializing OpenGL Window
	GLuint vertexArrayObject = NULL; // Need to specify how the blob of bits (the buffer) is decoded into vertexes. That is the job of the array.
	GLuint vertexArrayBuffer = NULL; // A buffer object in OpenGL is a big blob of bits

	glGenVertexArrays(1, &vertexArrayObject); // Initialize vertex array, passedby reference
	/*
		glBindVertexArray(array) binds the vertex array object with name array.
		array is the name of a vertex array object previously returned from a call to glGenVertexArrays
	*/
	glBindVertexArray(vertexArrayObject); // Bind VAO

	glGenBuffers(1, &vertexArrayBuffer); // Generate 1 buffer, put the resulting identifier in vertexArrayBuffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer); // The following commands will talk about our 'vertexArraybuffer' buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW); // give vertices data to OpenGL 

	GLuint shader = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader"); // Create and compile shader from tutorial

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{

		glClearColor(0.f, 0.f, 0.f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader); // use shader from tutorial

		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexArrayBuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);
		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	system("pause");
	return 0;
}

#endif // DEBUG