#pragma once
#include "do_not_edit.h"

void ClearColourBuffer(float col[4])
{
	for (int i = 0; i < *(&colour_buffer + 1) - colour_buffer; i++) {
		colour_buffer[i] = 1.f;
	}
}

void ClearDepthBuffer()
{
	for (int i = 0; i < *(&depth_buffer + 1) - depth_buffer; i++) {
		depth_buffer[i] = 0.f;
	}
}

void ApplyTransformationMatrix(glm::mat4 T, vector<triangle>& tris)
{
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
	
}
