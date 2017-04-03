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

/* prep function*/
GLFWwindow* initWindow();
GLfloat* initVertices();
void key_callback(GLFWwindow* window, int key, int scancode, int action,
		int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();

/* global parameter */
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

	/* key function */

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	Shader ourShader("shader/default.vs", "shader/default.frag");
	/* texture */
	glActiveTexture(GL_TEXTURE0);
	Texture texture1("texture/container.jpg");
	glActiveTexture(GL_TEXTURE1);
	Texture texture2("texture/awesomeface.png");
//	GLfloat vertices[] = {
//	// Positions // Colors // Texture Coords
//			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top Right
//			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
//			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom Left
//			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f // Top Left
//			};
	glm::vec3 cubePositions[] = { glm::vec3(0.0f, 0.0f, 0.0f), //
	glm::vec3(2.0f, 5.0f, -15.0f), //
	glm::vec3(-1.5f, -2.2f, -2.5f), //
	glm::vec3(-3.8f, -2.0f, -12.3f), //
	glm::vec3(2.4f, -0.4f, -3.5f), //
	glm::vec3(-1.7f, 3.0f, -7.5f), //
	glm::vec3(1.3f, -2.0f, -2.5f), //
	glm::vec3(1.5f, 2.0f, -2.5f), //
	glm::vec3(1.5f, 0.2f, -1.5f), //
	glm::vec3(-1.3f, 1.0f, -1.5f) };
	GLfloat vertices[] = { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	//
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f,	//
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	//
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	//
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,	//
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,	//

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	//
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,	//
			0.5f, 0.5f, 0.5f, 1.0f, 1.0f,	//
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,	//
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//

			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//
			-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	//
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//
			-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	//
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//
			0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//
			0.5f, -0.5f, -0.5f, 1.0f, 1.0f,	//
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	//
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f,	//
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,	//
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,	//

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,	//
			0.5f, 0.5f, -0.5f, 1.0f, 1.0f,	//
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f,	//
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,	//
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f };

	// Note that we start from 0!
	GLfloat indices[] { 0, 1, 3, // First Triangle
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), NULL);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
//			(GLvoid*) (3 * sizeof(GLfloat)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));
	// 0.1. Copy our index array in a element buffer for OpenGL to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
	GL_STATIC_DRAW);

	// 1. Then set the vertex attributes pointers
	glEnableVertexAttribArray(0);
//	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	// unbind VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// 2. Unbind the VAO
	glBindVertexArray(0);
	// 3. Use our shader program when we want to render an object
	glEnable(GL_DEPTH_TEST);
	// Game loop
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		do_movement();
		// a state-setting function: set clear color
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Be sure to activate the shader
		ourShader.Use();
		// Bind Textures using texture units
		glActiveTexture(GL_TEXTURE0);
		texture1.use(ourShader.Program, 0);
		glActiveTexture(GL_TEXTURE1);
		texture2.use(ourShader.Program, 1);
		/* GLM */
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glBindVertexArray(VAO);
		for (GLuint i = 0; i < 10; i++) {
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 2.0f * i;
			model = glm::rotate(model, (GLfloat) glfwGetTime() * angle,
					glm::vec3(1.0f, 0.3f, 0.5f));
			GLuint modelLoc = glGetUniformLocation(ourShader.Program, "model");
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glm::mat4 view;
			view = camera.GetViewMatrix();
			GLuint viewLoc = glGetUniformLocation(ourShader.Program, "view");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glm::mat4 projection;
			projection = glm::perspective(camera.Zoom,
					(GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 1000.0f);
			GLuint projectionLoc = glGetUniformLocation(ourShader.Program,
					"projection");
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
					glm::value_ptr(projection));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
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
	if (key == GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS){
		keys[key] = true;
		if(key == GLFW_KEY_SPACE){
			fall = false;
		}
	}

	else if (action == GLFW_RELEASE){
		keys[key] = false;
		if(key == GLFW_KEY_ESCAPE){
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
		camera.ProcessKeyboard(FALL,deltaTime);
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
