#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
#include "load_object.h"
#include <vector>
#include <algorithm>
using namespace std;

struct SCamera
{
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	bool Jumping;
	float Height;
	double jHeight;
	double groundOffset;

	const float MovementSpeed = 2.f;
	float MouseSensitivity = 0.05f;



};


void InitCamera(SCamera& in)
{
	in.Front = glm::vec3(0.0f, 0.0f, -1.0f);
	in.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
	in.WorldUp = in.Up;
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
	in.Jumping = false;
	in.Yaw = -90.f;
	in.Pitch = 0.f;
	in.Height = 2.0f;
	in.jHeight = 0.f;
	in.groundOffset = 0;
}

void MoveAndOrientCamera(SCamera& in, float xoffset, float zoffset, float xpos, float ypos, int maxx, int maxy)
{
	in.Yaw += in.MouseSensitivity * double((double)maxx / 2 - xpos);
	in.Pitch += in.MouseSensitivity * double((double)maxy / 2 - ypos);

	if (in.Pitch > 89.f) in.Pitch = 89.f;
	if (in.Pitch < -89.f) in.Pitch = -89.f;

	in.Front = glm::vec3(
		glm::cos(glm::radians(in.Pitch)) * glm::sin(glm::radians(in.Yaw)),
		glm::sin(glm::radians(in.Pitch)),
		glm::cos(glm::radians(in.Pitch)) * glm::cos(glm::radians(in.Yaw))
	);
	in.Right = glm::vec3(
		glm::sin(glm::radians(in.Yaw - 90)),
		0,
		glm::cos(glm::radians(in.Yaw - 90))
	);
	in.Up = glm::cross(in.Right, in.Front);

	//Trace a false ray to floor to calculate height addition for collisions

	in.Position += in.Front * zoffset * 1.5f * in.MovementSpeed;
	if (in.Position.z < -30)
		in.Position.z = -30;
	in.Position += in.Right * xoffset * in.MovementSpeed;
	in.Position.y = in.jHeight + in.Height + in.groundOffset;
}

double yCalc(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, float x, float z) {
	double det = (v2.z - v3.z) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.z - v3.z);

	double l1 = ((v2.z - v3.z) * (x - v3.x) + (v3.x - v2.x) * (z - v3.z)) / det;
	double l2 = ((v3.z - v1.z) * (x - v3.x) + (v1.x - v3.x) * (z - v3.z)) / det;
	double l3 = 1.0f - l1 - l2;

	return l1 * v1.y + l2 * v2.y + l3 * v3.y;
}

//https://uysalaltas.github.io/2022/03/24/OpenGL_Collision_Detection.html
bool checkIntersect(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
	glm::vec3 a0 = a - p;
	glm::vec3 b0 = b - p;
	glm::vec3 c0 = c - p;

	glm::vec3 normPBC = glm::cross(b0, c0);
	glm::vec3 normPCA = glm::cross(c0, a0);
	glm::vec3 normPAB = glm::cross(a0, b0);

	if (glm::dot(normPBC, normPCA) < 0.0f) {
		return false;
	}
	else if (glm::dot(normPBC, normPAB) < 0.0f) {
		return false;
	}

	return true;
}


//Calculate how much to bump the camera up by.
//Cam above 3 vertices AND vertices form a face.
//TODO: set abs(y) for when z is > 10
void CalculateGroundOffset(SCamera& in, std::vector<Vertex> vertices, std::vector<Face> faces) {
	glm::vec3 d = in.WorldUp * -1.0f;
	std::map<double, glm::vec3> sortedVertices;
	float u, v, w;
	for (int index = 0; index < vertices.size(); index++) {
		//Closest to camera's position
		glm::vec3 v = in.Position - vertices[index].position;
		double l = glm::length(v);
		sortedVertices[l] = vertices[index].position;
	}
	//Closest. Find all other vertices that share a face
	glm::vec3 closest = sortedVertices.begin()->second;
	std::vector<Face> facesFiltered;
	std::copy_if(faces.begin(), faces.end(), std::back_inserter(facesFiltered), [closest](Face iFace) {return (iFace.v1 == closest || iFace.v2 == closest || iFace.v3 == closest); });
	for (auto& face : facesFiltered) {
		if (checkIntersect(in.Position, face.v1, face.v2, face.v3)) {
			if(in.Position.z > 5)
				in.groundOffset = abs(yCalc(face.v1, face.v2, face.v3, in.Position.x, in.Position.z));
			else
				in.groundOffset = yCalc(face.v1, face.v2, face.v3, in.Position.x, in.Position.z);
			break;
		}
	}
}	

/**
Makes the camera play the Jump animation.
Run on second thread to allow other inputs to continue.
*/
void Jump(SCamera &in) { 
	double lastFrameTime = glfwGetTime();
	while (in.jHeight < 1) {
		double cFrameTime = glfwGetTime();
		double frameTimeDif = double(cFrameTime - lastFrameTime);
		in.jHeight += 8 * frameTimeDif;
		lastFrameTime = glfwGetTime();
	}
	while (in.jHeight > 0) {
		double cFrameTime = glfwGetTime();
		double frameTimeDif = double(cFrameTime - lastFrameTime);
		in.jHeight -= 8 * frameTimeDif;
		lastFrameTime = glfwGetTime();
	}
	in.jHeight = 0;
	in.Jumping = false;
}

