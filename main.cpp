#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <cmath>
// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include <SOIL.h>
// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mode);
// The MAIN function, from here we start the application and run the game loop
int main() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	/* Init GLFW */
	glfwInit();
	/* Set GLFW options */
	// version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// profile mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// resizable?
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyFirstRectangle",
			nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	/* GLEW */
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	/* Define the viewport dimensions */
	int width[2], height[2];
	// Automatically get the buffer size
	//glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, WIDTH, HEIGHT);

	/* key function */
	glfwSetKeyCallback(window, key_callback);

	Shader ourShader("shader/default.vs", "shader/default.frag");

	/* texture */
	glActiveTexture(GL_TEXTURE0);
	Texture texture1("texture/container.jpg");
	glActiveTexture(GL_TEXTURE1);
	Texture texture2("texture/awesomeface.png");
	GLfloat vertices[] = {
	// Positions // Colors // Texture Coords
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left
			};
	GLuint indices[] = {
	// Note that we start from 0!
			0, 1, 3, // First Triangle
			1, 2, 3 // Second Triangle
			};
	// Buffer ID
	GLuint VBO, VAO, EBO, colours_vbo = 0;

	// generate one with a buffer ID
	glGenBuffers(1, &VBO);

	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	// -1 . Bind Vertex Array Object
	glBindVertexArray(VAO);
	// 0. Copy our vertices array in a buffer for OpenGL to use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
			(GLvoid*) (6 * sizeof(GLfloat)));
	// 0.1. Copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	GL_STATIC_DRAW);

	// 1. Then set the vertex attributes pointers
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 2. Unbind the VAO
	glBindVertexArray(0);
	// 3. Use our shader program when we want to render an object

	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		// a state-setting function: set clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		// Be sure to activate the shader
		ourShader.Use();
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		texture1.use(ourShader.Program,0);
		glActiveTexture(GL_TEXTURE1);
		texture2.use(ourShader.Program,1);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	// delete vertex info
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	/* terminate */
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwPollEvents();
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
	}
}
