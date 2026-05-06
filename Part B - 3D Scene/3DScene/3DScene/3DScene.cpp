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
#include "load_object.h"
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
float seaboxVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//Back - sea
	-0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f, -0.5f,  6,0,
	 0.5f,  0.f, -0.5f,  6,6,
	 0.5f,  0.f, -0.5f,  6,6,
	-0.5f,  0.f, -0.5f,  0,6,
	-0.5f, -0.5f, -0.5f,  0,0,

	//Left cut this
	-0.5f,  0.f,  0.25f,  50,50,
	-0.5f,  0.f, -0.5f,  0,50,
	-0.5f, -0.5f, -0.5f,  0,0,
	-0.5f, -0.5f, -0.5f,  0,0,
	-0.5f, -0.5f,  0.25f,  50,0,
	-0.5f,  0.f,  0.25f,  50,50,

	//Right cut this at zero 
	 0.5f,  0.f,  0.25f,  50,50,
	 0.5f,  0.f, -0.5f,  0,50,
	 0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f, -0.5f,  0,0,
	 0.5f, -0.5f,  0.25f,  50,0,
	 0.5f,  0.f,  0.25f,  50,50,
};
float seaVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//bl
	 5.0f, -0.25f,  5.0f,  10,10,		
	//fl
	-5.0f, -0.25f,  5.0f,  0,10,		
	//fr
	-5.0f, 0.25f, -5.0f,  0,0,			

	//bl
	 5.0f, -0.25f,  5.0f,  10,10,		
	//fr
	-5.0f, 0.25f, -5.0f,  0,0,			
	//br
	 5.0f, 0.25f, -5.0f,  10,0,			
};

SCamera cam;
STerrain sea;
STerrain land;
float FoV = 45.f;
double oldX, oldY;
int maxx, maxy;
int renderDist = 50;

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}


void checkXSpawnTerrain(vector<Vertex>& vs, vector<Face>& fs, vector<Vertex> nvs, vector<Face> nfs, int intX) {
	//Positive X
	if (intX > 0) {
		intX = intX + 50;
		for (int i = (2 * intX) / 200; i > 0; i--) {
			
			for (auto v : nvs) {
				v.position.x = v.position.x + i * 200;
				vs.push_back(v);
			}
			for (auto f : nfs) {
				f.v1.x = f.v1.x + i * 200;
				f.v2.x = f.v2.x + i * 200;
				f.v3.x = f.v3.x + i * 200;
				fs.push_back(f);
			}
		}
	}
	//Negative X
	else {
		intX = intX - 50;
		for (int i = (2 * intX) / 200; i < 0; i++) {

			for (auto v : nvs) {
				v.position.x = v.position.x + i * 200;
				vs.push_back(v);
			}
			for (auto f : nfs) {
				f.v1.x = f.v1.x + i * 200;
				f.v2.x = f.v2.x + i * 200;
				f.v3.x = f.v3.x + i * 200;
				fs.push_back(f);
			}
		}
	}
}



void processKeyboard(GLFWwindow* window, vector<Vertex> landVertices, vector<Face> landFaces)
{
	static double lastFrameTime = glfwGetTime();
	double cFrameTime = glfwGetTime();
	double frameTimeDif = double(cFrameTime - lastFrameTime);
	
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
		thread t(Jump, ref(cam));
		t.detach();
	}
	CalculateGroundOffset(cam, landVertices, landFaces);
	MoveAndOrientCamera(cam, xoffset, zoffset, xpos, ypos, maxx, maxy);
	oldX = xpos;
	oldY = ypos;
	lastFrameTime = cFrameTime;
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);
	GLFWwindow* window = glfwCreateWindow(1920, 1080, "3D Scene - Maxim Carr - psymc9", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	unsigned int shaderProgram = CompileShader("triangle.vert", "triangle.frag");

	InitCamera(cam);
	

	//Get terrain and set spawn = a gentle slope
	std::vector<Vertex> landVertices;
	 
	std::vector<Face> landFaces;

	std::vector<Vertex> gentleSlopeVertices;
	std::vector<Face> gentleSlopeFaces;

	std::vector<Vertex> flatVertices;
	std::vector<Face> flatFaces;

	obj_parse(gentleSlopeVertices, gentleSlopeFaces, "objs/bumpy_plane.obj");
	obj_parse(flatVertices, flatFaces, "objs/flat_bumpy_plane.obj");

	landVertices = gentleSlopeVertices;
	landFaces = gentleSlopeFaces;

	GLuint sandTexture = setup_texture("textures/sand.bmp");
	GLuint nightSkyTexture = setup_texture("textures/night_sky.bmp");
	GLuint soilTexture = setup_texture("textures/soil_cracked.bmp");
	GLuint waterTexture = setup_texture("textures/water.png");
	GLuint waterTextureOpaque = setup_texture("textures/water_opaque.png");

	unsigned int VAO[5];
	glGenVertexArrays(5, VAO);
	unsigned int VBO[5];
	glGenBuffers(5, VBO);

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
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*landVertices.size(), &landVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//sea
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaVertices), seaVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//seabox
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaboxVertices), seaboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//flat
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * flatVertices.size(), &flatVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glUseProgram(shaderProgram);

	sea.tSize = 10;
	sea.tSpeed = 1.f;

	//Waves animation for water
	thread waves(Scroll, ref(sea));
	waves.detach();

	processKeyboard(window, landVertices, landFaces);
	MoveAndOrientCamera(cam, 1, 0, maxx/2, maxy/2, maxx, maxy);
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

		//skybox
		glBindTexture(GL_TEXTURE_2D, nightSkyTexture);
		glBindVertexArray(VAO[0]);
		model = glm::translate(model, cam.Position);
		//skybox size = 50 X 50 X 50 unit
		model = glm::scale(model, glm::vec3(2*renderDist, 2*renderDist, 2*renderDist));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Seabox
		model = glm::mat4(1.f);
		glBindTexture(GL_TEXTURE_2D, waterTextureOpaque);
		glBindVertexArray(VAO[3]);
		model = glm::translate(model, glm::vec3(cam.Position.x, -.5f, cam.Position.z));
		//skybox size = 50 X 50 X 50 unit
		model = glm::scale(model, glm::vec3(2 * renderDist -1, 2 * renderDist-1, 2 * renderDist-1));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 42);

		//sand
		
		landVertices = gentleSlopeVertices;
		landFaces = gentleSlopeFaces;
		checkXSpawnTerrain(landVertices, landFaces, gentleSlopeVertices, gentleSlopeFaces, cam.Position.x);
		printf("%f\n", cam.Position.x);
		printf("%d\n", landVertices.size());
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * landVertices.size(), &landVertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, sandTexture);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, landVertices.size());

		//sea
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		glBindVertexArray(VAO[2]);
		sea.positions.clear();
		for (int x = min(float(-renderDist), cam.Position.x - renderDist); x <= max(float(renderDist), cam.Position.x + renderDist)+10; x += 10) {
			for (int z = -renderDist-20; z <= renderDist+20; z += 10) {
				sea.positions.push_back(glm::vec3(x, -0, z));
				sea.positions.push_back(glm::vec3(x, -0.25, z));
				sea.positions.push_back(glm::vec3(x, -0.5, z));
				sea.positions.push_back(glm::vec3(x, -0.75, z));
				sea.positions.push_back(glm::vec3(x, -1, z));
				sea.positions.push_back(glm::vec3(x, -1.5, z));
				sea.positions.push_back(glm::vec3(x, -2, z));
				sea.positions.push_back(glm::vec3(x, -2.5, z));
				sea.positions.push_back(glm::vec3(x, -3, z));
				sea.positions.push_back(glm::vec3(x, -4.0, z));
				sea.positions.push_back(glm::vec3(x, -5, z));
				sea.positions.push_back(glm::vec3(x, -6, z));
				sea.positions.push_back(glm::vec3(x, -7, z));
				sea.positions.push_back(glm::vec3(x, -8.0, z));
			}
		}
		std::map<float, glm::vec3> sorted;
		for (int seI = 0; seI < sea.positions.size(); seI++) {
			glm::vec3 v = cam.Position - sea.positions[seI];
			float l = glm::length(v);
			sorted[l] = sea.positions[seI];
		}
		for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, it->second);
			model = glm::translate(model, glm::vec3(0, 0, sea.zOffset));
			if ((int)it->second.z % 20 != 0) {
				model = glm::rotate(model, glm::radians(180.f), glm::vec3(0,1,0));
			}
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glfwSwapBuffers(window);

		glfwPollEvents();
		processKeyboard(window, landVertices, landFaces);
	}

	glDeleteVertexArrays(5, VAO);
	glDeleteBuffers(5, VBO);

	glfwTerminate();

	return 0;
}


