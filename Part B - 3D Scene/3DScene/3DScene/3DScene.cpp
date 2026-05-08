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
#include "shadow.h"
#include <casteljau.h>
#include <ball.h>
#define WIDTH 1024
#define HEIGHT 768
#define SH_MAP_WIDTH 2048
#define SH_MAP_HEIGHT 2048
using namespace std;

float skyboxVertices[] = {
	// positions          // uv			//norm

	//-z
	-0.5f, -0.5f, -0.5f,  0,0,			0,0,1,
	 0.5f, -0.5f, -0.5f,  6,0,			0,0,1,
	 0.5f,  0.5f, -0.5f,  6,6,			0,0,1,

	 0.5f,  0.5f, -0.5f,  6,6,			0,0,1,
	-0.5f,  0.5f, -0.5f,  0,6,			0,0,1,
	-0.5f, -0.5f, -0.5f,  0,0,			0,0,1,

	//+z
	-0.5f, -0.5f,  0.5f,  0,0,			0,0,-1,
	0.5f,  0.5f,  0.5f,  6,6,			0,0,-1,
	 0.5f, -0.5f,  0.5f,  6,0,			0,0,-1,

	 0.5f,  0.5f,  0.5f,  6,6,			0,0,-1,
	 -0.5f, -0.5f,  0.5f,  0,0,			0,0,-1,
	-0.5f,  0.5f,  0.5f,  0,6,			0,0,-1,

	//-x
	-0.5f,  0.5f,  0.5f,  6,6,			1,0,0,
	-0.5f, -0.5f, -0.5f,  0,0,			1,0,0,
	-0.5f,  0.5f, -0.5f,  0,6,			1,0,0,
	
	-0.5f, -0.5f, -0.5f,  0,0,			1,0,0,
	-0.5f,  0.5f,  0.5f,  6,6,			1,0,0,
	-0.5f, -0.5f,  0.5f,  6,0,			1,0,0,
	

	//+x
	 0.5f,  0.5f,  0.5f,  6,6,			-1,0,0,
	 0.5f,  0.5f, -0.5f,  0,6,			-1,0,0,
	 0.5f, -0.5f, -0.5f,  0,0,			-1,0,0,
	 0.5f, -0.5f, -0.5f,  0,0,			-1,0,0,
	 0.5f, -0.5f,  0.5f,  6,0,			-1,0,0,
	 0.5f,  0.5f,  0.5f,  6,6,			-1,0,0,

	 //-y
	-0.5f, -0.5f, -0.5f,  0,0,			0,1,0,
	 0.5f, -0.5f, -0.5f,  6,0,			0,1,0,
	 0.5f, -0.5f,  0.5f,  6,6,			0,1,0,
	 0.5f, -0.5f,  0.5f,  6,6,			0,1,0,
	-0.5f, -0.5f,  0.5f,  0,6,			0,1,0,
	-0.5f, -0.5f, -0.5f,  0,0,			0,1,0,

	//+y
	-0.5f,  0.5f, -0.5f,  0,0,			0,-1,0,
	 0.5f,  0.5f, -0.5f,  6,0,			0,-1,0,
	 0.5f,  0.5f,  0.5f,  6,6,			0,-1,0,
	 0.5f,  0.5f,  0.5f,  6,6,			0,-1,0,
	-0.5f,  0.5f,  0.5f,  0,6,			0,-1,0,
	-0.5f,  0.5f, -0.5f,  0,0,			0,-1,0,
};
float seaboxVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//-z
	-0.5f, -0.5f, -0.5f,  0,0,			0,0,1,
	 0.5f, -0.5f, -0.5f,  6,0,			0,0,1,
	 0.5f,  0.f, -0.5f,  6,6,			0,0,1,
	 0.5f,  0.f, -0.5f,  6,6,			0,0,1,
	-0.5f,  0.f, -0.5f,  0,6,			0,0,1,
	-0.5f, -0.5f, -0.5f,  0,0,			0,0,1,

	//-x
	-0.5f,  0.f,  0.25f,  50,50,		1,0,0,
	-0.5f, -0.5f, -0.5f,  0,0,			1,0,0,
	-0.5f,  0.f, -0.5f,  0,50,			1,0,0,
	
	-0.5f, -0.5f, -0.5f,  0,0,			1,0,0,
	-0.5f,  0.f,  0.25f,  50,50,		1,0,0,
	-0.5f, -0.5f,  0.25f,  50,0,		1,0,0,
	
	//+x 
	 0.5f,  0.f,  0.25f,  50,50,		-1,0,0,
	 0.5f,  0.f, -0.5f,  0,50,			-1,0,0,
	 0.5f, -0.5f, -0.5f,  0,0,			-1,0,0,

	 0.5f, -0.5f, -0.5f,  0,0,			-1,0,0,
	 0.5f, -0.5f,  0.25f,  50,0,		-1,0,0,
	 0.5f,  0.f,  0.25f,  50,50,		-1,0,0,
};
float seaVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//tr
	 5.0f, -0.25f,  5.0f,  10,10,		0,.998752,.049938,
	 //br
	  5.0f, 0.25f, -5.0f,  10,0,		0,.998752,.049938,
	 //bl
	 -5.0f, 0.25f, -5.0f,  0,0,			0,.998752,.049938,
	
	//tr
	 5.0f, -0.25f,  5.0f,  10,10,		0,.998752,.049938,
	 //bl
	-5.0f, 0.25f, -5.0f,  0,0,			0,.998752,.049938,
	//tl
	-5.0f, -0.25f,  5.0f,  0,10,		0,.998752,.049938,
};
float moonVertices[] = {
	// positions          // uv			//SPECIFY ALPHA COORDINATE HERE
	//tr
		2.5f, 2.5f,  -1.f,  1,1,		0,0,1,
	 //bl
		-2.5f, -2.5f, -1.f,  0,0,		0,0,1,
	 //br
		2.5f, -2.5f, -1.f,  1,0,		0,0,1,


	  //tr
		2.5f, 2.5f,  -1.0f,  1,1,		0,0,1,
	   //tl
		-2.5f, 2.5f,  -1.0f,  0,1,		0,0,1,
	   //bl
		-2.5f, -2.5f, -1.f,  0,0,		0,0,1,

};

SCamera cam;
STerrain sea;
SBall ball;
glm::vec3 moonLightDir(-5.f, -10.f, 20.f);
glm::vec3 moonPos = -moonLightDir;
glm::vec3 flashLightColour(0,0,0);
glm::vec3 flashLightPos(0, 0, 0);
glm::vec3 flashLightDir;
float FoV = 45.f;
double oldX, oldY;
int renderDist = 50;
int tV = 0;

void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

//Beach specific
void checkXSpawnTerrain(vector<Vertex>& vs, vector<Face>& fs, vector<Vertex> nvs, vector<Face> nfs, glm::vec3 camPos) {
	//Positive X
	if (camPos.z < 100) {//No need to render sand when its out of view
		if (camPos.x > 0) {
			int intX = camPos.x + 90;
			for (int i = intX / 100; i >= 0; i--) {

				for (auto v : nvs) {
					v.position.x = v.position.x + i * 100;
					if (glm::length(camPos - v.position) < 340)//Def part of this terrain slice. Exclude others. 340 specifically to prevent terrain tearing 
						vs.push_back(v);
				}
				for (auto f : nfs) {
					f.v1.x = f.v1.x + i * 100;
					f.v2.x = f.v2.x + i * 100;
					f.v3.x = f.v3.x + i * 100;
					if (glm::length(camPos - f.v1) < 100 && glm::length(camPos - f.v2) < 100 && glm::length(camPos - f.v2) < 100)//Add face (def in slice) - not used for rendering just for collisions
						fs.push_back(f);
				}
			}
		}
		//Negative X
		else {
			int intX = camPos.x - 90;
			for (int i = intX / 100; i <= 0; i++) {

				for (auto v : nvs) {
					v.position.x = v.position.x + i * 100;
					if (glm::length(camPos - v.position) < 340)//Def part of this terrain slice. Exclude others. 340 specifically to prevent terrain tearing 
						vs.push_back(v);
				}
				for (auto f : nfs) {
					f.v1.x = f.v1.x + i * 100;
					f.v2.x = f.v2.x + i * 100;
					f.v3.x = f.v3.x + i * 100;
					if (glm::length(camPos - f.v1) < 100 && glm::length(camPos - f.v2) < 100 && glm::length(camPos - f.v2) < 100)//Add face (def in slice) - not used for rendering just for collisions
						fs.push_back(f);
				}
			}
		}
	}
}

//Grass specific
void checkZSpawnTerrain(vector<Vertex>& vs, vector<Face>& fs, vector<Vertex> nvs, vector<Face> nfs, glm::vec3 camPos) {
	int intZ = camPos.z + 90;
	for (int i = intZ / 100; i > 0; i--) {
		if (camPos.x >= 0) {
			int intX = camPos.x + 95;
			for (int j = intX / 100; j >= 0; j--) {

				for (auto v : nvs) {
					v.position.x = v.position.x + j * 100;
					v.position.z = v.position.z + i * 100;
					if (glm::length(camPos - v.position) < 340)//Def part of this terrain slice. Exclude others. 170 specifically to prevent terrain tearing 
						vs.push_back(v);
				}
				for (auto f : nfs) {
					f.v1.x = f.v1.x + j * 100;
					f.v1.z = f.v1.z + i * 100;
					f.v2.x = f.v2.x + j * 100;
					f.v2.z = f.v2.z + i * 100;
					f.v3.x = f.v3.x + j * 100;
					f.v3.z = f.v3.z + i * 100;
					if (glm::length(camPos - f.v1) < 100 && glm::length(camPos - f.v2) < 100 && glm::length(camPos - f.v2) < 100)//Add face (def in slice) - not used for rendering just for collisions
						fs.push_back(f);
				}
			}
		}
		//Negative X
		else {
			int intX = camPos.x - 95;
			for (int j = intX / 100; j <= 0; j++) {

				for (auto v : nvs) {
					v.position.x = v.position.x + j * 100;
					v.position.z = v.position.z + i * 100;
					if (glm::length(camPos - v.position) < 340)//Def part of this terrain slice. Exclude others. 170 specifically to prevent terrain tearing 
						vs.push_back(v);
				}
				for (auto f : nfs) {
					f.v1.x = f.v1.x + j * 100;
					f.v1.z = f.v1.z + i * 100;
					f.v2.x = f.v2.x + j * 100;
					f.v2.z = f.v2.z + i * 100;
					f.v3.x = f.v3.x + j * 100;
					f.v3.z = f.v3.z + i * 100;
					if (glm::length(camPos - f.v1) < 100 && glm::length(camPos - f.v2) < 100 && glm::length(camPos - f.v2) < 100)//Add face (def in slice) - not used for rendering just for collisions
						fs.push_back(f);
				}
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
	glfwSetCursorPos(window, (double)WIDTH / 2, (double)HEIGHT / 2);

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
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		flashLightDir = cam.Front;
		flashLightPos = cam.Position;
		flashLightColour = glm::vec3(2.55, 1.91, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE) {
		flashLightColour = glm::vec3(0, 0, 0);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
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
	MoveAndOrientCamera(cam, xoffset, zoffset, xpos, ypos, WIDTH, HEIGHT);
	oldX = xpos;
	oldY = ypos;
	lastFrameTime = cFrameTime;
}

void drawObjects(unsigned int VAO[17], unsigned int VBO[17], GLuint waterTexture, GLuint waterTextureOpaque, GLuint grassTexture, GLuint sandTexture, GLuint nightSkyTexture, GLuint shaderProgram, vector<Vertex>& landVerticesVar, vector<Face>& landFacesVar,
	vector<Vertex> gentleSlopeVerticesConst, vector<Face> gentleSlopeFacesConst, vector<Vertex> flatVerticesConst, vector<Face> flatFacesConst, vector<Vertex>& flatVerticesVar, GLuint ballTexture, GLuint moonTexture, GLuint treeTexture) {
	glUseProgram(shaderProgram);
	glm::mat4 model = glm::mat4(1.f); 
	//skybox
	glBindTexture(GL_TEXTURE_2D, nightSkyTexture);
	glBindVertexArray(VAO[0]);
	model = glm::translate(model, cam.Position);
	//skybox size = 100 X 100 X 100 unit
	model = glm::scale(model, glm::vec3(2 * renderDist, 2 * renderDist, 2 * renderDist));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	//Set type to 1 to ensure the stars always get drawn
	glUniform1i(glGetUniformLocation(shaderProgram, "type"), 1);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//Ball
	model = glm::mat4(1.f);
	glBindTexture(GL_TEXTURE_2D, ballTexture);
	CheckBall(ball, cam, landVerticesVar, landFacesVar);
	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * ball.myVs.size(), &ball.myVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
	glDrawArrays(GL_TRIANGLES, 0, ball.myVs.size());

	//Moon
	model = glm::mat4(1.f);
	model = glm::translate(model, moonPos);
	model = glm::translate(model, cam.Position);
	glBindTexture(GL_TEXTURE_2D, moonTexture);
	glBindVertexArray(VAO[15]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "type"), 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//Seabox
	model = glm::mat4(1.f);
	glBindTexture(GL_TEXTURE_2D, waterTextureOpaque);
	glBindVertexArray(VAO[3]);
	model = glm::translate(model, glm::vec3(cam.Position.x, -.5f, cam.Position.z));
	
	model = glm::scale(model, glm::vec3(2 * renderDist - 1, 2 * renderDist - 1, 2 * renderDist - 1));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 42);

	//sand
	landVerticesVar.clear();
	landFacesVar.clear();
	checkXSpawnTerrain(landVerticesVar, landFacesVar, gentleSlopeVerticesConst, gentleSlopeFacesConst, cam.Position);
	if (landVerticesVar.size() > 0) {
		glBindVertexArray(VAO[1]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * landVerticesVar.size(), &landVerticesVar[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, sandTexture);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
		glDrawArrays(GL_TRIANGLES, 0, landVerticesVar.size());
	}

	//grass/flatlands
	flatVerticesVar.clear();
	checkZSpawnTerrain(flatVerticesVar, landFacesVar, flatVerticesConst, flatFacesConst, cam.Position);
	if (flatVerticesVar.size() > 0) {
		glBindVertexArray(VAO[4]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * flatVerticesVar.size(), &flatVerticesVar[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindTexture(GL_TEXTURE_2D, grassTexture);
		model = glm::mat4(1.0f);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
		glDrawArrays(GL_TRIANGLES, 0, flatVerticesVar.size());
	}

	//tree
	glBindVertexArray(VAO[16]);
	glBindTexture(GL_TEXTURE_2D, treeTexture);
	for (int i = 0; i < landVerticesVar.size(); i++) {
		if (landVerticesVar[i].position.z > 10 && i % 24 == 0) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, landVerticesVar[i].position);
			model = glm::rotate(model, glm::radians((float)(i % 6 * 6)), glm::vec3(0.0f, 1.0f, .0f));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
			glDrawArrays(GL_TRIANGLES, 0, tV);
		}
	}
	for (int i = 0; i < flatVerticesVar.size(); i++) {
		if (i % 12 == 0) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, flatVerticesVar[i].position);
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
			glDrawArrays(GL_TRIANGLES, 0, tV);
		}
	}

	//sea
	glBindTexture(GL_TEXTURE_2D, waterTexture);
	glBindVertexArray(VAO[2]);
	sea.positions.clear();
	for (int x = min(float(-renderDist), cam.Position.x - renderDist); x <= max(float(renderDist), cam.Position.x + renderDist) + 10; x += 10) {
		for (int z = -renderDist - 20; z <= renderDist / 2; z += 10) {
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
			model = glm::rotate(model, glm::radians(180.f), glm::vec3(0, 1, 0));
			glFrontFace(GL_CCW);
		}
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(shaderProgram, "type"), 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix, unsigned int VAO[17], unsigned int VBO[17], GLuint waterTexture, GLuint waterTextureOpaque, GLuint grassTexture, GLuint sandTexture, GLuint nightSkyTexture, vector<Vertex>& landVerticesVar, vector<Face>& landFacesVar,
	vector<Vertex> gentleSlopeVerticesConst, vector<Face> gentleSlopeFacesConst, vector<Vertex> flatVerticesConst, vector<Face> flatFacesConst, vector<Vertex>& flatVerticesVar, GLuint ballTexture, GLuint moonTexture, GLuint treeTexture)
{
	glViewport(0, 0, SH_MAP_WIDTH, SH_MAP_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	drawObjects(VAO, VBO, waterTexture, waterTextureOpaque, grassTexture, sandTexture, nightSkyTexture, shadowShaderProgram, landVerticesVar, landFacesVar, gentleSlopeVerticesConst, gentleSlopeFacesConst, flatVerticesConst, flatFacesConst, flatVerticesVar, ballTexture, moonTexture, treeTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void renderWithShadow(unsigned int renderShadowProgram, ShadowStruct shadow, glm::mat4 projectedLightSpaceMatrix, unsigned int VAO[17], unsigned int VBO[17], GLuint waterTexture, GLuint waterTextureOpaque, GLuint grassTexture, GLuint sandTexture, GLuint nightSkyTexture, vector<Vertex>& landVerticesVar, vector<Face>& landFacesVar,
	vector<Vertex> gentleSlopeVerticesConst, vector<Face> gentleSlopeFacesConst, vector<Vertex> flatVerticesConst, vector<Face> flatFacesConst, vector<Vertex>& flatVerticesVar, GLuint ballTexture, GLuint moonTexture, GLuint treeTexture) {

	glViewport(0, 0, WIDTH, HEIGHT);

	static const GLfloat bgd[] = { 0.f, 0.f, .36f, 1.f };
	glClearBufferfv(GL_COLOR, 0, bgd);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUseProgram(renderShadowProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, shadow.Texture);
	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	glUniform3f(glGetUniformLocation(renderShadowProgram, "lightDirection"), moonLightDir.x, moonLightDir.y, moonLightDir.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "lightColour"), 1.f, 1.f, 1.f);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "camPos"), cam.Position.x, cam.Position.y, cam.Position.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "flashLightPos"), flashLightPos.x, flashLightPos.y, flashLightPos.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "flashLightCol"), flashLightColour.x, flashLightColour.y, flashLightColour.z);
	glUniform3f(glGetUniformLocation(renderShadowProgram, "flashLightDir"), flashLightDir.x, flashLightDir.y, flashLightDir.z);

	glm::mat4 view = glm::mat4(1.f);
	view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::mat4(1.f);
	projection = glm::perspective(glm::radians(FoV), (float)WIDTH / (float)HEIGHT, .1f, 100.f);
	glUniformMatrix4fv(glGetUniformLocation(renderShadowProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	drawObjects(VAO, VBO, waterTexture, waterTextureOpaque, grassTexture, sandTexture, nightSkyTexture, renderShadowProgram, landVerticesVar, landFacesVar, gentleSlopeVerticesConst, gentleSlopeFacesConst, flatVerticesConst, flatFacesConst, flatVerticesVar, ballTexture, moonTexture, treeTexture);
}

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_SAMPLES, 8);
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "3D Scene - Maxim Carr - psymc9", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	gl3wInit();

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback((GLDEBUGPROC)DebugCallback, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	ShadowStruct shadow = setup_shadowmap(SH_MAP_WIDTH, SH_MAP_HEIGHT);

	GLuint shaderProgram = CompileShader("triangle.vert", "triangle.frag");
	GLuint shadowProgram = CompileShader("shadow.vert", "shadow.frag");
	GLuint curveProgram = CompileShader("mvp.vert", "col.frag");

	int num_evals = 128;
	vector<point> ctrl_pts;

	int num_curve_verts[8];
	int num_curve_floats[8];
	float* curve_vertices[8];
	vector<point> bezierVectors[8];
	for (int i = 0; i < 8; i++) {
		ctrl_pts.clear();
		ctrl_pts.push_back(point(20.f, 40.f, 0.f));
		ctrl_pts.push_back(point(16.f, 40.f, glm::min((double)0, -1.25 + i * 0.125)));
		ctrl_pts.push_back(point(12.f, 40.f, glm::min((double)0, -1 + i * 0.125)));
		ctrl_pts.push_back(point(4.f, 40.f, glm::min((double)0, -0.25 + i * 0.0625)));
		ctrl_pts.push_back(point(-10.f, 40.f, 0.f));
		num_curve_verts[i] = 0;
		num_curve_floats[i] = 0;
		curve_vertices[i] = NULL;
		bezierVectors[i] = EvaluateBezierCurve(ctrl_pts, num_evals);
		curve_vertices[i] = MakeFloatsFromVector(bezierVectors[i], num_curve_verts[i], num_curve_floats[i], 1.f, 1.f, 1.f);
	}

	ctrl_pts.clear();
	//Progressively closer to centre
	ctrl_pts.push_back(point(20.f, 40.f, 0.f));
	ctrl_pts.push_back(point(16.f, 40.f, 0.f));
	ctrl_pts.push_back(point(12.f, 40.f, 0.f));
	ctrl_pts.push_back(point(4.f, 40.f, 0.f));
	ctrl_pts.push_back(point(-10.f, 40.f, 0.f));
	int num_curve_verts_sl = 0;
	int num_curve_floats_sl = 0;
	float* sl_curve_vertices = NULL;
	std::vector<point> sl = EvaluateBezierCurve(ctrl_pts, num_evals);
	sl_curve_vertices = MakeFloatsFromVector(sl, num_curve_verts_sl, num_curve_floats_sl, 1.f, 1.f, 1.f);

	InitCamera(cam);
	
	//Get terrain and set spawn = a gentle slope
	std::vector<Vertex> landVerticesVar;
	std::vector<Face> landFacesVar;
	std::vector<Vertex> flatVerticesConst;
	std::vector<Face> flatFacesConst;

	std::vector<Vertex> gentleSlopeVerticesConst;
	std::vector<Face> gentleSlopeFacesConst;

	std::vector<Vertex> flatVerticesVar;
	std::vector<Face> flatFacesVar;

	std::vector<Vertex> ballVertices;
	std::vector<Face> ballFaces;

	std::vector<Vertex> treeVertices;
	std::vector<Face> treeFaces;

	obj_parse(gentleSlopeVerticesConst, gentleSlopeFacesConst, "objs/bumpy_plane.obj");
	obj_parse(flatVerticesConst, flatFacesConst, "objs/flat_bumpy_plane.obj");
	obj_parse(ballVertices, ballFaces, "objs/ball.obj");
	obj_parse(treeVertices, treeFaces, "objs/tree.obj");

	tV = treeVertices.size();

	landVerticesVar = gentleSlopeVerticesConst;
	landFacesVar = gentleSlopeFacesConst;
	flatVerticesVar = flatVerticesConst;
	flatFacesVar = flatFacesConst;

	GLuint sandTexture = setup_texture("textures/sand.bmp");
	GLuint nightSkyTexture = setup_texture("textures/night_sky.bmp");
	GLuint grassTexture = setup_texture("textures/grass.bmp");
	GLuint waterTexture = setup_texture("textures/water.png");
	GLuint waterTextureOpaque = setup_texture("textures/water_opaque.png");
	GLuint ballTexture = setup_texture("textures/ball_texture.png");
	GLuint moonTexture = setup_texture("textures/moon.png");
	GLuint treeTexture = setup_texture("textures/tree.png");

	SetupBall(ball, ballVertices, landVerticesVar, landFacesVar);

	unsigned int VAO[17];
	glGenVertexArrays(17, VAO);
	unsigned int VBO[17];
	glGenBuffers(17, VBO);

	//INCLUDE ALPHA COORDINATE IN THE VAO HERE
	//skybox
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//land
	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*landVerticesVar.size(), &landVerticesVar[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//sea
	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaVertices), seaVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//seabox
	glBindVertexArray(VAO[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(seaboxVertices), seaboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//flat
	glBindVertexArray(VAO[4]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * flatVerticesVar.size(), &flatVerticesVar[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Bezier comet
	for (int i = 5; i <= 12; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_curve_floats[i-5], &curve_vertices[i-5][0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}
	glBindVertexArray(VAO[13]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[13]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* num_curve_floats_sl, &sl_curve_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Ball
	glBindVertexArray(VAO[14]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[14]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* ball.myVs.size(), &ball.myVs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//Moon
	glBindVertexArray(VAO[15]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[15]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(moonVertices), moonVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//Tree
	glBindVertexArray(VAO[16]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[16]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)* tV, &treeVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//glUseProgram(shaderProgram);

	sea.tSize = 10;
	sea.tSpeed = 1.f;

	//Waves animation for water
	thread waves(Scroll, ref(sea));
	waves.detach();

	processKeyboard(window, landVerticesVar, landFacesVar);
	MoveAndOrientCamera(cam, 1, 0, WIDTH/2, HEIGHT/2, WIDTH, HEIGHT);
	while (!glfwWindowShouldClose(window))
	{
		glm::mat4 model(1.f);
		float near_plane = 0.1f, far_plane = 150.f;
		model = glm::translate(model, cam.Position);
		glm::mat4 lProj = glm::ortho(75.f, -75.f, -8.f, 50.f, near_plane, far_plane);
		glm::mat4 lView = glm::lookAt(moonPos, cam.Position, glm::vec3(0, 1, 0));
		glm::mat4 lPVMat = lProj * lView * model;

		generateDepthMap(shadowProgram, shadow, lPVMat, VAO, VBO, waterTexture, waterTextureOpaque, grassTexture, sandTexture, nightSkyTexture, landVerticesVar, landFacesVar, gentleSlopeVerticesConst, gentleSlopeFacesConst, flatVerticesConst, flatFacesConst, flatVerticesVar, ballTexture, moonTexture, treeTexture);


		renderWithShadow(shaderProgram, shadow, lPVMat, VAO, VBO, waterTexture, waterTextureOpaque, grassTexture, sandTexture, nightSkyTexture, landVerticesVar, landFacesVar, gentleSlopeVerticesConst, gentleSlopeFacesConst, flatVerticesConst, flatFacesConst, flatVerticesVar, ballTexture, moonTexture, treeTexture);

		glUseProgram(curveProgram);
		model = glm::mat4(1.f);
		model = glm::translate(model, cam.Position);
		glUniformMatrix4fv(glGetUniformLocation(curveProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));


		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(cam.Position, cam.Position + cam.Front, cam.Up);
		glUniformMatrix4fv(glGetUniformLocation(curveProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(FoV), (float)WIDTH / (float)HEIGHT, .1f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(curveProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (int i = 5; i <= 12; i++) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_LINE_STRIP, 0, num_curve_verts[i-5]);
		}
		glBindVertexArray(VAO[13]);
		glDrawArrays(GL_LINE_STRIP, 0, num_curve_verts_sl);
		model = glm::scale(model, glm::vec3(1, 1, -1));
		glUniformMatrix4fv(glGetUniformLocation(curveProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
		for (int i = 5; i <= 12; i++) {
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_LINE_STRIP, 0, num_curve_verts[i - 5]);
		}

		glfwSwapBuffers(window);

		glfwPollEvents();
		landVerticesVar.insert(landVerticesVar.end(), flatVerticesVar.begin(), flatVerticesVar.end());
		processKeyboard(window, landVerticesVar, landFacesVar);
	}

	glDeleteVertexArrays(17, VAO);
	glDeleteBuffers(17, VBO);

	glfwTerminate();

	return 0;
}