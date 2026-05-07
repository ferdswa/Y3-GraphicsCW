#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
#include <vector>
#include <algorithm>
struct SBall {
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
};