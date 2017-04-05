#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <cmath>
// GLFW
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include <SOIL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ObjLoader.h"
/* prep function*/
GLFWwindow* initWindow();
GLfloat* initVertices();
void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

/* global parameter */

// Light attributes
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
const GLuint WIDTH = 800, HEIGHT = 600;
bool keys[1024];
bool fall;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = false;
GLfloat yaw = -90.0f; // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right (due to how Eular angles work) so we initially rotate a bit to the left.
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
GLfloat deltaTime = 0.0f; // Time between current frame and last frame
GLfloat lastFrame = 0.0f; // Time of last frame
GLfloat fov = 45.0f;
// Window dimensions

// The MAIN function, from here we start the application and run the game loop
int objLoaderTest() {
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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting", nullptr,
			nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
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
	// Automatically get the buffer size
	//glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, WIDTH, HEIGHT);

	Shader defaultShader("shader/objLoaderTest.vs", "shader/objLoaderTest.frag");

	/* ObjLoader */
	ObjLoader objLoader("obj/sphere.obj");
	objLoader.info();
	GLfloat *vertices = objLoader.getVertices();
	GLuint *indices = objLoader.getIndices();
	const int p = objLoader.getNumberOfPoints() * 3;
	const int q = objLoader.getNumberOfTriangle() * 3;
	// First, set the container's VAO (and VBO)
	GLuint VBO, EBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, p* sizeof(GLfloat), vertices,
	GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, q * sizeof(GLuint), indices,
	GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
			(GLvoid*) 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* key function */

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 3. Use our shader program when we want to render an object
	glEnable(GL_DEPTH_TEST);
	// Game loop
	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();
		// a state-setting function: set clear color
		glClearColor(0.1f, 1.0f, 1.0f, 0.5f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Be sure to activate the shader
		defaultShader.Use();

		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom,
				(GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(defaultShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(defaultShader.Program, "view");
		GLint projLoc = glGetUniformLocation(defaultShader.Program,
				"projection");
		// Pass the matrices to the shader
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, q, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	// delete vertex info
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &VBO);
	/* terminate */
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();

	return 0;
}
void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mode) {
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS) {
		keys[key] = true;
		if (key == GLFW_KEY_SPACE) {
			fall = false;
		}
	}

	else if (action == GLFW_RELEASE) {
		keys[key] = false;
		if (key == GLFW_KEY_ESCAPE) {
			fall == true;
		}
	}

}
void do_movement() {
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.ProcessKeyboard(JUMP, deltaTime);
	if (fall == true)
		camera.ProcessKeyboard(FALL, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessMouseScroll(yoffset);
}

GLFWwindow* initWindow() {

	return nullptr;

}

GLfloat* initVertices() {

	return nullptr;
}
