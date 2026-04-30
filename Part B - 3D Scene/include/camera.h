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
	bool jumping;
	float Height;

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
	in.jumping = false;
	in.Yaw = -90.f;
	in.Pitch = 0.f;
}

float cam_dist = 2.f;

void MoveAndOrientCamera(SCamera& in, float xoffset, float yoffset, float zoffset, float xpos, float ypos, int maxx, int maxy)
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
	in.Position.y = yoffset + in.Height;
}

void Jump(SCamera &in, float xoffset, float zoffset, float xpos, float ypos, int maxx, int maxy) {
	static double lastTime = glfwGetTime();
	printf("jump start\n");
	while (in.Position.y < 1) {
		printf("jump up\n");
		MoveAndOrientCamera(in, xoffset, in.Position.y + 0.00034, zoffset, xpos, ypos, maxx, maxy);
	}
	while (in.Position.y > 0) {
		printf("jump down\n");
		MoveAndOrientCamera(in, xoffset, in.Position.y - 0.00034, zoffset, xpos, ypos, maxx, maxy);
	}
	MoveAndOrientCamera(in, xoffset, 0, zoffset, xpos, ypos, maxx, maxy);
	in.jumping = false;
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	printf("jump compl in %f\n", deltaTime);
}



void SimGravityOnCamera(SCamera& in, float deltaY) {

}