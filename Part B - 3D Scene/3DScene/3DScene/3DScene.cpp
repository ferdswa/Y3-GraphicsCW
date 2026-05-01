#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <map>

#include "camera.h"
#include "error.h"
#include "file.h"
#include "shader.h"
using namespace std;






float cubeVertices[] = {
	// positions          // col			//SPECIFY ALPHA COORDINATE HERE
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.f,
	-0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.f,

	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.f,
	 0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f,
	 0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f,
	-0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,

	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f,
	 0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
	-0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.f,
};
float planeVertices[] = {
	// positions          // col			//SPECIFY ALPHA COORDINATE HERE
	 5.0f, -0.5f,  5.0f,  1.0f, 1.0f, 1.f,
	-5.0f, -0.5f,  5.0f,  1.0f, 1.0f, 1.f,
	-5.0f, -0.5f, -5.0f,  1.0f, 1.0f, 1.f,

	 5.0f, -0.5f,  5.0f,  1.0f, 1.0f, 1.f,
	-5.0f, -0.5f, -5.0f,  1.0f, 1.0f, 1.f,
	 5.0f, -0.5f, -5.0f,  1.0f, 1.0f, 1.f,
};


float transparentVertices[] = {
	// positions				// RGB				//SPECIFY ALPHA COORDINATE HERE
	-.5f,  -0.5f,  0.0f,		1.0f,  0.0f, 0.0f,
	.0f, 0.5f,  0.0f,			1.0f,  0.0f, 0.0f,
	.5f, -0.5f,  0.0f,			1.0f,  0.0f, 0.0f,
};

SCamera cam;
float FoV = 45.f;
double oldX, oldY;
int maxx, maxy;

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}






void processKeyboard(GLFWwindow* window)
{
	static double lastFrameTime = glfwGetTime();
	double cFrameTime = glfwGetTime();
	float frameTimeDif = float(cFrameTime - lastFrameTime);
	
	float xoffset = 0.f, zoffset = 0.f;
	double xpos, ypos;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//Get and reset cursor pos
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwGetWindowSize(window, &maxx, &maxy);
	glfwSetCursorPos(window, (double)maxx / 2, (double)maxy / 2);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		zoffset = 2.f * frameTimeDif;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		zoffset = -2.f * frameTimeDif;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		xoffset = -2.f * frameTimeDif;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		xoffset = 2.f * frameTimeDif;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
		xoffset /= 2;
		zoffset /= 2;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
		//Replace with height test for bumpy plane
		cam.Height = 1.75f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
		cam.Height = 2.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !cam.Jumping) {
		//jump logic
		cam.Jumping = true;
		thread t(Jump, ref(cam), frameTimeDif);
		t.detach();
	}
	MoveAndOrientCamera(cam, xoffset, zoffset, xpos, ypos, maxx, maxy);
	oldX = xpos;
	oldY = ypos;
	lastFrameTime = cFrameTime;
}


int main(int argc, char** argv)
{
	glfwInit();
	GLFWwindow* window = glfwCreateWindow(800, 600, "Transparency", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, 0);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//ENABLE BLENDING HERE

	//SET BLEND FUNCTION HERE

	unsigned int shaderProgram = CompileShader("triangle.vert", "triangle.frag");


	unsigned int VAO[3];
	glGenVertexArrays(3, VAO);
	unsigned int VBO[3];
	glGenBuffers(3, VBO);

	//INCLUDE ALPHA COORDINATE IN THE VAO HERE
	//cube
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//plane
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//transparent
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//


	glUseProgram(shaderProgram);

	std::vector<glm::vec3> positions;
	positions.push_back(glm::vec3(0, 0, 2.f));
	positions.push_back(glm::vec3(0.5f, 0, 0.f));
	positions.push_back(glm::vec3(0, 0, 1.f));
	positions.push_back(glm::vec3(-.5f, 0, 0.5f));
	positions.push_back(glm::vec3(0, 0, 1.5f));

	processKeyboard(window);
	while (!glfwWindowShouldClose(window))
	{
		static const GLfloat bgd[] = { .02f, .5f, .75f, 1.f };
		glClearBufferfv(GL_COLOR, 0, bgd);
		glClear(GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(FoV), (float)maxx/ (float)maxy, .1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.f);

		// cubes
		glBindVertexArray(VAO[0]);
		model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (float)glfwGetTime() / 2.f, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(.5, .5, .5));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (float)glfwGetTime() / -2.f, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(.5, .5, .5));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// floor
		glBindVertexArray(VAO[1]);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 6);


		//SORT THE TRANSPARENT TRIANGLE POSITIONS HERE

		glBindVertexArray(VAO[2]);
		for (int i = 0; i < positions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
		processKeyboard(window);
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

	glfwTerminate();

	return 0;
}

