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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lighting",
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

	Shader lightingShader("shader/lighting.vs", "shader/lighting.frag");
	Shader lampShader("shader/lamp.vs", "shader/lamp.frag");

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

	/* ObjLoader */
	ObjLoader objLoader("obj/sphere.obj");
	std::cout<<objLoader.getNumberOfTriangle()<<std::endl;
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
	GLfloat vertices[] = { -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //
			0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //
			0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //
			-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, //

			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
			0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
			0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
			-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, //

			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, //
			-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, //
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, //
			-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, //
			-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, //
			-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, //

			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //
			0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //
			0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, //
			0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //
			0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, //

			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, //
			0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, //
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, //
			0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, //
			-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, //
			-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, //

			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //
			0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, //
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //
			0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //
			-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, //
			-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f };

	// Note that we start from 0!
	GLfloat indices[] { 0, 1, 3, // First Triangle
			1, 2, 3 // Second Triangle
	};
	// First, set the container's VAO (and VBO)
	GLuint VBO, containerVAO;
	glGenVertexArrays(1, &containerVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(containerVAO);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(GLvoid*) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(GLvoid*) (3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	// Then, we set the light's VAO (VBO stays the same. After all, the vertices are the same for the light object (also a 3D cube))
	GLuint lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// We only need to bind to the VBO (to link it with glVertexAttribPointer), no need to fill it; the VBO's data already contains all we need.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set the vertex attributes (only position data for the lamp))
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
			(GLvoid*) 0);

	glEnableVertexAttribArray(0);

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
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Be sure to activate the shader
		lightingShader.Use();
		glm::mat4 tempRotate;
		glm::vec4 tempLightPos = glm::rotate(tempRotate, (GLfloat) glfwGetTime() * 2.0f,
				glm::vec3(0.0, 1.0, 0.0))*glm::vec4(lightPos,1.0f);
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program,
				"objectColor");
		GLint lightColorLoc = glGetUniformLocation(lightingShader.Program,
				"lightColor");
		GLint lightPosLoc = glGetUniformLocation(lightingShader.Program,
				"lightPos");
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program,
				"viewPos");
		glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y,
				camera.Position.z);
		glUniform3f(lightPosLoc, tempLightPos.x, tempLightPos.y, tempLightPos.z);
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 0.5f, 1.0f);
		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom,
				(GLfloat) WIDTH / (GLfloat) HEIGHT, 0.1f, 100.0f);
		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program,
				"projection");
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Draw the container (using container's vertex attributes)
		glBindVertexArray(containerVAO);
		glm::mat4 model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4();
		model = glm::rotate(model, (GLfloat) glfwGetTime() * 2.0f,
				glm::vec3(0.0, 1.0, 0.0));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glfwSwapBuffers(window);
	}
	// delete vertex info
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteVertexArrays(1, &containerVAO);
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
