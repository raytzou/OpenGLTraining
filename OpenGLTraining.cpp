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
/////Tutorial resource/////
#include <common/shader.cpp>

using namespace std;
using namespace glm;

const unsigned short SCREEN_WIDTH = 800;
const unsigned short SCREEN_HEIGHT = 600;

static const GLfloat g_vertex_buffer_data[] =
{
	-1.f, -1.f, 0.f, // (-1, -1),
	1.f, -1.f, 0.f, // (1, -1)
	0.f, 1.f, 0.f // (0, 1)
};

int main(void)
{
	cout << "hello OpenGL" << endl;

	if (!glfwInit()) // Initialize GLFW library. Have to do this first, otherwise APIs won't work.
	{
		cout << "Initializing GLFW library failed." << endl;
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x MSAA
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
#ifdef MACOS
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // I don't like to make MacOS happy
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

#pragma region TriangleVAO
	GLuint VertexArrayID = NULL;
	glGenVertexArrays(1, &VertexArrayID); // Initialize vertex array, passedby reference
	glBindVertexArray(VertexArrayID); // Bind VAO
	GLuint shader = LoadShaders("SimpleTransform.vertexshader", "SingleColor.fragmentshader");
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
#pragma endregion

#pragma region MyMatrixTraining
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
#pragma endregion

#pragma region AllMatrices
	mat4 projectionMatrix = perspective(
		radians(30.0f), //FoV 30 degree
		4.0f / 3.0f, // 4:3 aspect ratio
		0.1f, // for near clipping plane
		100.0f // farest cliiping plane, 0.1 unit to 100.0 unit
	);
	mat4 cameraMatrix = lookAt(
		vec3(4.0f, 3.0f, 3.0f), // cameraPosition = (4,3,3), viewed in perspective from point (4,3,3)
		vec3(.0f, .0f, .0f), // targetPosition = (0,0,0)
		vec3(.0f, 1.0f, .0f) // Head is up (set to 0,-1,0 to look upside-down)
	);
	mat4 modelMatrix = mat4(1.0f); // Model matrix : an identity matrix (model will be at the origin)
	mat4 modelViewProjectionMatrix = projectionMatrix * cameraMatrix * modelMatrix;
#pragma endregion

	GLuint MatrixID = glGetUniformLocation(shader, "MVP"); // use GLSL APIs to get handler of MVP uniform

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glClearColor(.0f, .0f, .0f, .0f); // Not like glClear(), it doesn't clear the buffer. More like set color. glClearColor() is State Setting

	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader); // use shader from tutorial
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &modelViewProjectionMatrix[0][0]); // send current ID of transformation to shader in MVP uniform

#pragma region DrawTriangle
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
		glDisableVertexAttribArray(0);
#pragma endregion

		glfwSwapBuffers(window); // swap front and back buffer
		glfwPollEvents(); // poll and process events
	}

#pragma region CleanupVAO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(shader);
	glDeleteVertexArrays(1, &VertexArrayID);
#pragma endregion

	glfwTerminate();
	system("pause");
	return 0;
}

