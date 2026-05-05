#pragma once
#include <glm/glm.hpp>
#include <list>

int pixelPoses[10][10];
struct STerrain {
	vector<glm::vec3> positions;

	int tSize = 1;
	float tSpeed = 0.f;
	double zOffset = 0.f;
};
void Scroll(STerrain& in) {
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

//void SetupLand(STerrain& in) {
//	srand(452026);
//	int blU = 0, blV = 0;
//	int brU = 2, brV = 0;
//	int tlU = 0, tlV = 2;
//	int trU = 2, trV = 2;
//	
//	//Get pixel poses
//	//for (int x = 0; x < 1000; x += in.tSize) {
//	//	for (int z = 0; z < 40; z += in.tSize) {
//	//		pixelPoses[x+500][z+552]= -1 * (rand() % 3 + 5);
//	//	}
//	//	for (int z = 40; z < 50; z += in.tSize) {
//	//		pixelPoses[x + 500][z + 552] = -1 * (rand() % 8 + 0);
//	//	}
//	//	for (int z = 50; z < 1052; z += in.tSize) {
//	//		pixelPoses[x + 500][z + 552] = (rand() % 10 + 0);
//	//	}
//	//}
//	//for (int x = 0; x < 1000; x += 2) {
//	//	int lX = x, rX = x + 2;
//	//	for (int z = 0; z < 1052; z += 2) {
//	//		int bZ = z, tZ = z + 2;
//	//		int blY = pixelPoses[lX+500][bZ+552], brY = pixelPoses[rX+500][bZ+552], tlY = pixelPoses[lX+500][tZ+552], trY = pixelPoses[rX+500][tZ+552];
//
//	//		//Add vertices
//	//		resVertices.push_back(lX);
//	//		resVertices.push_back(blY);
//	//		resVertices.push_back(bZ);
//	//		resVertices.push_back(blU);
//	//		resVertices.push_back(blV);
//
//	//		resVertices.push_back(rX);
//	//		resVertices.push_back(brY);
//	//		resVertices.push_back(bZ);
//	//		resVertices.push_back(brU);
//	//		resVertices.push_back(brV);
//
//	//		resVertices.push_back(lX);
//	//		resVertices.push_back(tlY);
//	//		resVertices.push_back(tZ);
//	//		resVertices.push_back(tlU);
//	//		resVertices.push_back(tlV);
//
//	//		resVertices.push_back(lX);
//	//		resVertices.push_back(tlY);
//	//		resVertices.push_back(tZ);
//	//		resVertices.push_back(tlU);
//	//		resVertices.push_back(tlV);
//
//	//		resVertices.push_back(rX);
//	//		resVertices.push_back(brY);
//	//		resVertices.push_back(bZ);
//	//		resVertices.push_back(brU);
//	//		resVertices.push_back(brV);
//
//	//		resVertices.push_back(rX);
//	//		resVertices.push_back(trY);
//	//		resVertices.push_back(tZ);
//	//		resVertices.push_back(trU);
//	//		resVertices.push_back(trV);
//	//	}
//	//}
//	//return resVertices;
//}
//vector<float> Setup(STerrain in) {
//	//srand(452026);
//	int blU = 0, blV = 0;
//	int brU = 2, brV = 0;
//	int tlU = 0, tlV = 2;
//	int trU = 2, trV = 2;
//	vector<float> resVertices;
//	for (int x = 0; x < 10; x += 2) {
//		for (int z = 0; z < 10; z += 2) {
//			pixelPoses[x][z] = (rand() % 5 + 5);
//		}
//		/*for (int z = 40; z < 50; z += in.tSize) {
//			pixelPoses[x][z] =  (rand() % 5 + 5);
//		}
//		for (int z = 50; z < 1052; z += in.tSize) {
//			pixelPoses[x][z] = (rand() % 10 + 0);
//		}*/
//	}
//	for (int x = 0; x < 10; x += 2) {
//		int lX = x, rX = x + 2;
//		for (int z = 0; z < 10; z += 2) {
//			int bZ = z, tZ = z + 2;
//			int blY = pixelPoses[lX][bZ], brY = pixelPoses[rX][bZ], tlY = pixelPoses[lX][tZ], trY = pixelPoses[rX][tZ];
//
//			//Add vertices
//			resVertices.push_back(lX);
//			resVertices.push_back(blY);
//			resVertices.push_back(bZ);
//			resVertices.push_back(blU);
//			resVertices.push_back(blV);
//
//			resVertices.push_back(rX);
//			resVertices.push_back(brY);
//			resVertices.push_back(bZ);
//			resVertices.push_back(brU);
//			resVertices.push_back(brV);
//
//			resVertices.push_back(lX);
//			resVertices.push_back(tlY);
//			resVertices.push_back(tZ);
//			resVertices.push_back(tlU);
//			resVertices.push_back(tlV);
//
//			resVertices.push_back(lX);
//			resVertices.push_back(tlY);
//			resVertices.push_back(tZ);
//			resVertices.push_back(tlU);
//			resVertices.push_back(tlV);
//
//			resVertices.push_back(rX);
//			resVertices.push_back(brY);
//			resVertices.push_back(bZ);
//			resVertices.push_back(brU);
//			resVertices.push_back(brV);
//
//			resVertices.push_back(rX);
//			resVertices.push_back(trY);
//			resVertices.push_back(tZ);
//			resVertices.push_back(trU);
//			resVertices.push_back(trV);
//		}
//	}
//	return resVertices;
//}