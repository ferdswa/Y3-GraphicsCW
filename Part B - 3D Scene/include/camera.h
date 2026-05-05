#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
#include "load_object.h"
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
	if (in.Position.z < -20)
		in.Position.z = -20;
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

//Christer Ericson
void Barycentric(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c, float& u, float& v, float& w)
{
	glm::vec3 v0 = b - a, v1 = c - a, v2 = p - a;
	float d00 = glm::dot(v0, v0);
	float d01 = glm::dot(v0, v1);
	float d11 = glm::dot(v1, v1);
	float d20 = glm::dot(v2, v0);
	float d21 = glm::dot(v2, v1);
	float denom = d00 * d11 - d01 * d01;
	v = (d11 * d20 - d01 * d21) / denom;
	w = (d00 * d21 - d01 * d20) / denom;
	u = 1.0f - v - w;
}


//Calculate how much to bump the camera up by.
//Cam above 3 vertices AND vertices form a face.
void CalculateGroundOffset(SCamera& in, std::vector<Vertex> vertices) {
	glm::vec3 d = in.WorldUp * -1.0f;
	std::map<double, glm::vec3> sortedVertices;
	for (int index = 0; index < vertices.size(); index++) {
		//Closest to camera's horiz position
		glm::vec2 v = glm::vec2(in.Position.x, in.Position.z) - glm::vec2(vertices[index].position.x, vertices[index].position.z);
		double l = glm::length(v);
		sortedVertices[l] = vertices[index].position;
	}
	//Closest. Find all other vertices that share a face
	glm::vec3 closest = sortedVertices.begin()->second;
	
	
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

