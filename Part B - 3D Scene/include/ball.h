#pragma once
#include <stdio.h>
#include <glm/glm.hpp>
#include <thread>
#include <vector>
#include <algorithm>
#include <load_object.h>

struct SBall {
	std::vector<Vertex> myVs;
	std::vector<Vertex> constVs;
	glm::vec3 myPos;
};
//As this is a sphere, pos can be calculated by taking the average of all vertices
void calcPos(SBall& ball) {
	glm::vec3 sum(0, 0, 0);
	glm::vec3 avg(0, 0, 0);

	for (const Vertex v : ball.myVs) {
		sum = sum + v.position;
	}
	avg = sum / (float)ball.myVs.size();
	ball.myPos = avg;
}

void calcVs(SBall& ball) {
	vector<Vertex> newVs;
	Vertex v;

	for (int i = 0; i < ball.myVs.size(); i++) {
		v = ball.myVs[i];
		v.position = ball.constVs[i].position + ball.myPos;
		newVs.push_back(v);
	}
	ball.myVs = newVs;
}

//Calculate how much to bump the camera up by.
//Cam above 3 vertices AND vertices form a face.
double CalculateGroundOffsetBall(SBall& in, std::vector<Vertex> vertices, std::vector<Face> faces) {
	glm::vec3 d = -glm::vec3(0, 1, 0);
	std::map<double, glm::vec3> sortedVertices;
	double groundOffset;
	for (int index = 0; index < vertices.size(); index++) {
		//Closest to camera's position
		glm::vec3 v = glm::vec3(in.myPos.x, 0, in.myPos.z) - glm::vec3(vertices[index].position.x, 0, vertices[index].position.z);
		double l = glm::length(v);
		sortedVertices[l] = vertices[index].position;
	}
	//Closest. Find all other vertices that share a face
	glm::vec3 closest = sortedVertices.begin()->second;
	std::vector<Face> facesFiltered;
	std::copy_if(faces.begin(), faces.end(), std::back_inserter(facesFiltered), [closest](Face iFace) {return (iFace.v1 == closest || iFace.v2 == closest || iFace.v3 == closest); });
	for (auto& face : facesFiltered) {
		if (checkIntersect(in.myPos, face.v1, face.v2, face.v3)) {
			if (in.myPos.z > 5) {
				groundOffset = abs(yCalc(face.v1, face.v2, face.v3, in.myPos.x, in.myPos.z));
				return groundOffset;
			}
			else {
				groundOffset = yCalc(face.v1, face.v2, face.v3, in.myPos.x, in.myPos.z);
				return groundOffset;
			}
			break;
		}
	}
	return 0;
}
void Drop(SBall& ball, std::vector<Vertex> landVertices, std::vector<Face> landFaces) {
	double initSpeedY = -0.1;
	double groundOffset;
	ball.myPos.y = ball.myPos.y + initSpeedY;
	groundOffset = CalculateGroundOffsetBall(ball, landVertices, landFaces);
	double lastFrameTime = glfwGetTime();
	while (ball.myPos.y > groundOffset + 0.01) {
		double cFrameTime = glfwGetTime();
		double frameTimeDif = double(cFrameTime - lastFrameTime);
		initSpeedY -= 2 * frameTimeDif;
		ball.myPos.y = ball.myPos.y + initSpeedY;
		groundOffset = CalculateGroundOffsetBall(ball, landVertices, landFaces);
		calcVs(ball);
		lastFrameTime = glfwGetTime();
	}
	printf("finish", ball.myPos.y);
	
}

void Kick(SBall& ball, glm::vec3 forceVec, std::vector<Vertex> landVertices, std::vector<Face> landFaces) {
	forceVec = glm::normalize(forceVec);
	double initSpeedX = forceVec.x;
	double initSpeedY = 1;
	double initSpeedZ = forceVec.z;
	double groundOffset = 0;
	ball.myPos.x = ball.myPos.x + initSpeedX;
	ball.myPos.y = ball.myPos.y + initSpeedY;
	ball.myPos.z = ball.myPos.z + initSpeedZ;
	groundOffset = CalculateGroundOffsetBall(ball, landVertices, landFaces);
	double lastFrameTime = glfwGetTime();
	while (initSpeedY>0) {
		double cFrameTime = glfwGetTime();
		double frameTimeDif = double(cFrameTime - lastFrameTime);
		
		if (abs(initSpeedX) > 0.025) {
			if (initSpeedX > 0) {
				initSpeedX -= 0.000025;
			}
			else {
				initSpeedX += 0.000025;
			}
		}
		else {
			initSpeedX = 0;
		}
		if(abs(initSpeedZ) > 0.025) {
			if (initSpeedZ > 0) {
				initSpeedZ -= 0.000025;
			}
			else {
				initSpeedZ += 0.000025;
			}
		}
		else {
			initSpeedZ = 0;
		}
		initSpeedY -= 0.025;
		ball.myPos.x = ball.myPos.x + initSpeedX * frameTimeDif;
		ball.myPos.y = ball.myPos.y + initSpeedY * frameTimeDif ;
		ball.myPos.z = ball.myPos.z + initSpeedZ * frameTimeDif;
		groundOffset = CalculateGroundOffsetBall(ball, landVertices, landFaces);
		calcVs(ball);
		lastFrameTime = glfwGetTime();
	}
	calcVs(ball);

	ball.myPos.y += CalculateGroundOffsetBall(ball, landVertices, landFaces);
	thread drop(Drop, ref(ball), landVertices, landFaces);
	drop.detach();
}
void CheckBall(SBall& ball, SCamera cam, std::vector<Vertex> landVertices, std::vector<Face> landFaces) {
	//cam.Position.y = cam.Position.y - cam.Height;
	glm::vec3 relV = ball.myPos - glm::vec3(cam.Position.x, cam.Position.y-cam.Height, cam.Position.z);
	double length = glm::length(relV);
	printf("%f\n", length);
	if (length <= 0.5) {//Kick ball
		thread kick(Kick, ref(ball), relV, landVertices, landFaces);
		kick.detach();
	}
}
void SetupBall(SBall& ball, std::vector<Vertex> vs, std::vector<Vertex> landVertices, std::vector<Face> landFaces) {
	ball.myVs = vs;
	ball.constVs = vs;
	calcPos(ball);
	thread drop(Drop, ref(ball), landVertices, landFaces);
	drop.detach();
	//printf("\n%f, %f, %f\n", ball.myPos.x, ball.myPos.y, ball.myPos.z);
}

