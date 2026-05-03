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
#include "texture.h"
#include "terrain.h"
using namespace std;






float skyboxVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE

	//back?
	-0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f, -0.5f,  6,0,
	 0.5f,  0.5f, -0.5f,  6,6,
	 0.5f,  0.5f, -0.5f,  6,6,
	-0.5f,  0.5f, -0.5f,  0,6,
	-0.5f, -0.5f, -0.5f,  0,0,

	//Front?
	-0.5f, -0.5f,  0.5f,  0,0,
	 0.5f, -0.5f,  0.5f,  6,0,
	 0.5f,  0.5f,  0.5f,  6,6,
	 0.5f,  0.5f,  0.5f,  6,6,
	-0.5f,  0.5f,  0.5f,  0,6,
	-0.5f, -0.5f,  0.5f,  0,0,

	//Left
	-0.5f,  0.5f,  0.5f,  6,6,
	-0.5f,  0.5f, -0.5f,  0,6,
	-0.5f, -0.5f, -0.5f,  0,0,
	-0.5f, -0.5f, -0.5f,  0,0,
	-0.5f, -0.5f,  0.5f,  6,0,
	-0.5f,  0.5f,  0.5f,  6,6,

	//Right
	 0.5f,  0.5f,  0.5f,  6,6,
	 0.5f,  0.5f, -0.5f,  0,6,
	 0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f,  0.5f,  6,0,
	 0.5f,  0.5f,  0.5f,  6,6,

	 //bottom
	-0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f, -0.5f,  6,0,
	 0.5f, -0.5f,  0.5f,  6,6,
	 0.5f, -0.5f,  0.5f,  6,6,
	-0.5f, -0.5f,  0.5f,  0,6,
	-0.5f, -0.5f, -0.5f,  0,0,

	//top
	-0.5f,  0.5f, -0.5f,  0,0,
	 0.5f,  0.5f, -0.5f,  6,0,
	 0.5f,  0.5f,  0.5f,  6,6,
	 0.5f,  0.5f,  0.5f,  6,6,
	-0.5f,  0.5f,  0.5f,  0,6,
	-0.5f,  0.5f, -0.5f,  0,0,
};
float sandVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//bl
	 0.5f, 0.f,  0.5f,  1,1,
	 //fl
	-0.5f, 0.f,  0.5f,  0,1,
	//fr
	-0.5f, 0.f, -0.5f,  0,0,

	//bl
	 0.5f, 0.f,  0.5f,  1,1,
	//fr
	-0.5f, 0.f, -0.5f,  0,0,
	//br
	 0.5f, 0.f, -0.5f,  1,0,
};
float seaVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//bl
	 5.0f, -0.25f,  5.0f,  10,10,
	//fl
	-5.0f, -0.25f,  5.0f,  0,10,
	//fr
	-5.0f, -0.25f, -5.0f,  0,0,

	//bl
	 5.0f, -0.25f,  5.0f,  10,10,
	//fr
	-5.0f, -0.25f, -5.0f,  0,0,
	//br
	 5.0f, -0.25f, -5.0f,  10,0,
};

//float transparentVertices[] = {
//	// positions				// RGB				//SPECIFY ALPHA COORDINATE HERE
//	-.5f,  -0.5f,  0.0f,		1.0f,  0.0f, 0.0f,
//	.0f, 0.5f,  0.0f,			1.0f,  0.0f, 0.0f,
//	.5f, -0.5f,  0.0f,			1.0f,  0.0f, 0.0f,
//};

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
		cam.Height = 1.5f;
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
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Scene - Maxim Carr - psymc9", NULL, NULL);
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

	InitCamera(cam);

	GLuint sandTexture = setup_texture("textures/sand.bmp");
	GLuint nightSkyTexture = setup_texture("textures/night_sky.bmp");
	GLuint soilTexture = setup_texture("textures/soil_cracked.bmp");
	GLuint waterTexture = setup_texture("textures/water.bmp");

	unsigned int VAO[3];
	glGenVertexArrays(3, VAO);
	unsigned int VBO[3];
	glGenBuffers(3, VBO);

	//INCLUDE ALPHA COORDINATE IN THE VAO HERE
	//skybox
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//sand
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sandVertices), sandVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//sea
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaVertices), seaVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//


	glUseProgram(shaderProgram);


	vector<glm::vec3> sandPos;
	vector<glm::vec3> seaPos;

	sandPos.push_back(glm::vec3(0, 0, 0));
	sandPos.push_back(glm::vec3(1, 0, 0));
	sandPos.push_back(glm::vec3(1, 0, 1));
	sandPos.push_back(glm::vec3(0, 0, 1));
	sandPos.push_back(glm::vec3(-1, 0, 0));
	sandPos.push_back(glm::vec3(-1, 0, 1));
	sandPos.push_back(glm::vec3(-1, 0, -1));
	sandPos.push_back(glm::vec3(1, 0, -1));
	sandPos.push_back(glm::vec3(0, 0, -1));

	processKeyboard(window);
	while (!glfwWindowShouldClose(window))
	{
		static const GLfloat bgd[] = { 0.f, 0.f, .36f, 1.f };
		glClearBufferfv(GL_COLOR, 0, bgd);
		glClear(GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT, GL_FILL);


		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(FoV), (float)maxx/ (float)maxy, .1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.f);

		// cubes
		glBindTexture(GL_TEXTURE_2D, nightSkyTexture);
		glBindVertexArray(VAO[0]);
		model = glm::translate(model, cam.Position);
		//skybox size = 50 X 50 X 50 unit
		model = glm::scale(model, glm::vec3(100, 100, 100));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -1.0f));
		model = glm::rotate(model, (float)glfwGetTime() / -2.f, glm::vec3(0.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(.5, .5, .5));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);*/

		//sand
		glBindTexture(GL_TEXTURE_2D, sandTexture);
		glBindVertexArray(VAO[1]);
		for (int saI = 0; saI < sandPos.size(); saI++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, sandPos[saI]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//sea
		seaPos.clear();
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		glBindVertexArray(VAO[2]);
		for (int x = int(cam.Position.x) - 50; x < int(cam.Position.x) + 50; x += 10) {
			for (int z = int(cam.Position.z) - 50; z < int(cam.Position.z) + 50; z += 10) {
				seaPos.push_back(glm::vec3(x, 0, z));
			}
		}
		model = glm::mat4(1.0f);
		for (int seI = 0; seI < seaPos.size(); seI++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, seaPos[seI]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}

		//SORT THE TRANSPARENT TRIANGLE POSITIONS HERE

		/*glBindVertexArray(VAO[2]);
		for (int i = 0; i < positions.size(); i++)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, positions[i]);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 3);
		}*/

		glfwSwapBuffers(window);

		glfwPollEvents();
		processKeyboard(window);
	}

	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);

	glfwTerminate();

	return 0;
}

