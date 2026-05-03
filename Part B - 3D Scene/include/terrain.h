#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <cmath>
#include <set>
#include <algorithm>

void CheckAndUpdateSea(vector<glm::vec3> positions, glm::vec3 camPos) {
	float l;
	std::map<float, glm::vec3> sorted;
	for (int i = 0; i < positions.size(); i++) {
		glm::vec3 v = camPos - positions[i];
		l = glm::length(v);
		sorted[l] = positions[i];
	}
	auto furthest = sorted.begin();
	if (furthest->first < 50) {//Furthest is closer than skybox dist
		if (furthest->second.x > camPos.x) {
			positions.push_back(glm::vec3(furthest->second.x + 10));
			CheckAndUpdateSea(positions, camPos);
		}
		if (furthest->second.x < camPos.x) {
			positions.push_back(glm::vec3(furthest->second.x - 10));
			CheckAndUpdateSea(positions, camPos);
		}
		if (furthest->second.z < camPos.z) {
			positions.push_back(glm::vec3(furthest->second.z - 10));
			CheckAndUpdateSea(positions, camPos);
		}
		if (furthest->second.z > camPos.z) {
			positions.push_back(glm::vec3(furthest->second.z + 10));
		}
	}
}