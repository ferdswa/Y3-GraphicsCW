#pragma once
#include "do_not_edit.h"

void ClearColourBuffer(float col[4])
{
	for (int i = 0; i < *(&colour_buffer + 1) - colour_buffer; i++) {
		colour_buffer[i] = col[i%4];
	}
}

void ClearDepthBuffer()
{
	for (int i = 0; i < *(&depth_buffer + 1) - depth_buffer; i++) {
		depth_buffer[i] = 0.f;
	}
}

void ApplyTransformationMatrix(glm::mat4 T, vector<triangle>& tris)//Position
{
	for (triangle& tri: tris) {
		tri.v1.pos = T * tri.v1.pos;
		tri.v2.pos = T * tri.v2.pos;
		tri.v3.pos = T * tri.v3.pos;
	}
}

void ApplyPerspectiveDivision(vector<triangle>& tris)
{
	
}

void ApplyViewportTransformation(int w, int h, vector<triangle>& tris)
{
}


void ComputeBarycentricCoordinates(int px, int py, triangle t, float& alpha, float& beta, float& gamma)
{
}

void ShadeFragment(triangle tri, float& alpha, float& beta, float& gamma, glm::vec3& col, float& depth)
{
}


void Rasterise(vector<triangle> tris)
{
	for (int py = 0; py < PIXEL_H; py++)
	{
		float percf = (float)py / (float)PIXEL_H;
		int perci = percf * 100;
		std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;

		for (int px = 0; px < PIXEL_W; px++)
		{
		}
	}
	std::clog << "\rFinish rendering.           \n";
}

void render(vector<triangle>& tris)
{
	float col[] = { 1.f,1.f,1.f,1.f };
	glm::mat4 projMtx = glm::mat4(1.0f);
	glm::mat4 viewMtx = glm::mat4(1.0f);
	glm::mat4 modelMtx = glm::mat4(1.0f);
	//Clear the buffers
	ClearColourBuffer(col);
	ClearDepthBuffer();
	//Apply transformations
	ApplyTransformationMatrix(modelMtx, tris);
	viewMtx = glm::translate(viewMtx, glm::vec3(0.1, -2.5, -6));
	ApplyTransformationMatrix(viewMtx, tris);
	projMtx = glm::perspective(55.f, (float)PIXEL_W / PIXEL_H, 0.1f, 10.f);
	ApplyTransformationMatrix(projMtx, tris);





}
