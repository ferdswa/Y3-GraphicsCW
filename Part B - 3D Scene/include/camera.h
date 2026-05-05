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

void CalculateGroundOffset(SCamera& in, std::vector<Vertex> vertices) {
	glm::vec3 d = in.WorldUp * -1.0f;
	std::map<float, glm::vec3> sortedVertices;
	for (const auto& vert : vertices) {
		//Closest to camera's horiz position
		glm::vec3 v = in.Position - vert.position;
		v.y = 0;
		float l = glm::length(v);
		sortedVertices[l] = vert.position;
	}
	//Closest 3
	glm::vec3 closest3[3];
	int i = 0;
	for (auto it = sortedVertices.begin(); it != sortedVertices.end(); it++) {
		closest3[i] = it->second;
		if (i == 2)
			break;
		i++;
	}
	glm::vec3 v12 = closest3[1] - closest3[0];
	glm::vec3 v13 = closest3[2] - closest3[0];
	glm::vec3 psubo = closest3[0] - in.Position;

	glm::vec3 res = glm::normalize(glm::cross(v12, v13));
	float resF = glm::dot(psubo, res);
	float denom = glm::dot(d, res);
	float t = resF / denom;
	glm::vec3 dt = d * t;
	glm::vec3 point = in.Position + dt;

	in.groundOffset = point.y;
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



void SimGravityOnCamera(SCamera& in, float deltaY) {

}