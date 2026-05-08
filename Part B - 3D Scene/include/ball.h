#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
#include <vector>
#include <algorithm>
#include <load_object.h>

struct SBall {
	std::vector<Vertex> myVs;
	glm::vec3 myPos;
};
//As this is a sphere, pos can be calculated by taking the average of all vertices
void calcPos(SBall& ball) {
	glm::vec3 sum(0, 0, 0);
	glm::vec3 avg(0, 0, 0);

	for (Vertex v : ball.myVs) {
		sum = sum + v.position;
	}
	avg = sum / (float)ball.myVs.size();
	ball.myPos = avg;
}
void SetupBall(SBall& ball, std::vector<Vertex> vs) {
	ball.myVs = vs;
	calcPos(ball);
}

