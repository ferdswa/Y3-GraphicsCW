#pragma once

#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
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

	const float MovementSpeed = .5f;
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
	float Height = 0;
	in.Yaw = -90.f;
	in.Pitch = 0.f;
}

float cam_dist = 2.f;

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

	in.Position += in.Front * zoffset;
	in.Position += in.Right * xoffset;
	in.Position.y = in.jHeight + in.Height;// + groundOffset (when impl)
}

/**
Makes the camera play the Jump animation.
Run on second thread to allow other inputs to continue.
*/
void Jump(SCamera &in, float dTime) {
	while (in.jHeight < 1) {
		
		in.jHeight += 0.000002 * dTime;
	}
	while (in.jHeight > 0) {
		in.jHeight -= 0.000002 * dTime;
	}
	in.jHeight = 0;
	in.Jumping = false;
}



void SimGravityOnCamera(SCamera& in, float deltaY) {

}