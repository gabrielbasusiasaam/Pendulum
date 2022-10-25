#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "Shader.h"


void framebuffer_size_callback(GLFWwindow*, int width, int height);
void process_input(GLFWwindow* window);
void create_pendulum(std::vector<float>& storage, glm::vec3 position, float length, float angle);
void forward_step(std::vector<float>& pendulums, float gravity, float time_step, int count);
GLFWwindow* init_window(float window_size[]);

int main()
{
	float window_size[] = { 500.0f, 500.0f };
	float pi = 3.14159;
	float g = 1;
	float dt = 1.0f / 60.0f;
	int count = 2;

	GLFWwindow* window = init_window(window_size);
	std::vector<float> pendulums(0);
	if (window == NULL) return -1;
	glm::vec3 position = glm::vec3(0.0f, 0.75f, 0.0f);
	create_pendulum(pendulums, position, 1.2, pi * 0.3);
	create_pendulum(pendulums, -position, 0.2, pi * 0.2);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(1);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &pendulums[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader line_shader = Shader("vertexShader.vert", "fragmentShader.frag", "geometryShader.geom");
	Shader bob_shader = Shader("vertexShader.vert", "fragmentShader.frag", "geometryShader2.geom");

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		process_input(window);

		forward_step(pendulums, g, dt, count);
		
		line_shader.use();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, count * 6 * sizeof(float), &pendulums[0], GL_DYNAMIC_DRAW);
		glDrawArrays(GL_POINTS, 0, count);

		bob_shader.use();
		glDrawArrays(GL_POINTS, 0, count);

		glfwSwapBuffers(window);
		glfwPollEvents();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}

	glfwTerminate();
	return 0;
}

void create_pendulum(std::vector<float>& storage, glm::vec3 position, float length, float angle) {
	storage.push_back(position.x);
	storage.push_back(position.y);
	storage.push_back(position.z);
	storage.push_back(length);
	storage.push_back(angle);
	storage.push_back(0);
}

void forward_step(std::vector<float>& pendulums, float gravity, float time_step, int count) {
	for (int i = 0; i < count; i++) {
		int index = 6 * i;
		pendulums[index + 4] = pendulums[index + 4] + pendulums[index + 5] * time_step;
		pendulums[index + 5] = pendulums[index + 5] - (gravity / pendulums[index + 3]) * sin(pendulums[index + 4]) * time_step;
	}
}



void framebuffer_size_callback(GLFWwindow*, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

GLFWwindow* init_window(float window_size[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(window_size[0], window_size[1], "LearnOpenGL", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create window!\n";
		glfwTerminate();
		return NULL;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialise GLAD\n";
		return NULL;
	}

	glViewport(0, 0, window_size[0], window_size[1]);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}