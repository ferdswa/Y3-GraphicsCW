#pragma once
#include <glm/glm.hpp>

struct STerrain {
	vector<glm::vec3> positions;

	int tSize = 1;
	float tSpeed = 0.f;
	double zOffset = 0.f;
};

float Scroll(STerrain& in) {
	static double lastFrameTime = glfwGetTime();
	while(true){
		double cFrameTime = glfwGetTime();
		double frameTimeDif = double(cFrameTime - lastFrameTime);
		in.zOffset += frameTimeDif * in.tSpeed;
		if (in.zOffset >= 2 * in.tSize) {
			in.zOffset = 0;
		}
		lastFrameTime = cFrameTime;
	}
}