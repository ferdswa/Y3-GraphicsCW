#pragma once
#include "do_not_edit.h"

//Scale? 

void ClearColourBuffer(float col[4])
{
	for (int i = 0; i < *(&colour_buffer + 1) - colour_buffer; i++) {
		colour_buffer[i] = 255.f;
	}
}

void ClearDepthBuffer()
{
	for (int i = 0; i < *(&depth_buffer + 1) - depth_buffer; i++) {
		depth_buffer[i] = INFINITY;
	}
}

void ApplyTransformationMatrix(glm::mat4 T, vector<triangle>& tris)//Split this up
{
	glm::vec4 newPos = glm::vec4(1.f);
	for (triangle& tri : tris) {
		newPos = T * tri.v1.pos;
		tri.v1.pos = newPos;
		newPos = T * tri.v2.pos;
		tri.v2.pos = newPos;
		newPos = T * tri.v3.pos;
		tri.v3.pos = newPos;


		tri.v1.nor[0] = tri.v1.pos[0];
		tri.v1.nor[1] = tri.v1.pos[1];
		tri.v1.nor[2] = tri.v1.pos[2];
		tri.v2.nor[0] = tri.v2.pos[0];
		tri.v2.nor[1] = tri.v2.pos[1];
		tri.v2.nor[2] = tri.v2.pos[2];
		tri.v3.nor[0] = tri.v3.pos[0];
		tri.v3.nor[1] = tri.v3.pos[1];
		tri.v3.nor[2] = tri.v3.pos[2];
	}
}

void ApplyPerspectiveDivision(vector<triangle>& tris)
{
	int w = 0;
	for (triangle& tri : tris) {
		//v1
		w = tri.v1.pos[3];
		tri.v1.pos[0] = tri.v1.pos[0] / w;
		tri.v1.pos[1] = tri.v1.pos[1] / w;
		tri.v1.pos[2] = tri.v1.pos[2] / w;
		tri.v1.pos[3] = tri.v1.pos[3] / w;
		//v2
		w = tri.v2.pos[3];
		tri.v2.pos[0] = tri.v2.pos[0] / w;
		tri.v2.pos[1] = tri.v2.pos[1] / w;
		tri.v2.pos[2] = tri.v2.pos[2] / w;
		tri.v2.pos[3] = tri.v2.pos[3] / w;
		//v3
		w = tri.v3.pos[3];
		tri.v3.pos[0] = tri.v3.pos[0] / w;
		tri.v3.pos[1] = tri.v3.pos[1] / w;
		tri.v3.pos[2] = tri.v3.pos[2] / w;
		tri.v3.pos[3] = tri.v3.pos[3] / w;

		tri.v1.nor[0] = tri.v1.pos[0];
		tri.v1.nor[1] = tri.v1.pos[1];
		tri.v1.nor[2] = tri.v1.pos[2];
		tri.v2.nor[0] = tri.v2.pos[0];
		tri.v2.nor[1] = tri.v2.pos[1];
		tri.v2.nor[2] = tri.v2.pos[2];
		tri.v3.nor[0] = tri.v3.pos[0];
		tri.v3.nor[1] = tri.v3.pos[1];
		tri.v3.nor[2] = tri.v3.pos[2];
	}
}

void ApplyViewportTransformation(int w, int h, vector<triangle>& tris)
{
	for (triangle& tri:tris) {
		//v1
		tri.v1.pos.x = (tri.v1.pos.x + 1) * (w/2.f);
		tri.v1.pos.y = (tri.v1.pos.y + 1) * (h/2.f);
		//v2
		tri.v2.pos[0] = (tri.v2.pos[0] + 1) * (w / 2.f);
		tri.v2.pos[1] = (tri.v2.pos[1] + 1) * (h / 2.f);
		//v3
		tri.v3.pos[0] = (tri.v3.pos[0] + 1) * (w / 2.f);
		tri.v3.pos[1] = (tri.v3.pos[1] + 1) * (h / 2.f);

		tri.v1.nor[0] = tri.v1.pos[0];
		tri.v1.nor[1] = tri.v1.pos[1];
		tri.v1.nor[2] = tri.v1.pos[2];
		tri.v2.nor[0] = tri.v2.pos[0];
		tri.v2.nor[1] = tri.v2.pos[1];
		tri.v2.nor[2] = tri.v2.pos[2];
		tri.v3.nor[0] = tri.v3.pos[0];
		tri.v3.nor[1] = tri.v3.pos[1];
		tri.v3.nor[2] = tri.v3.pos[2];
	}
}


void ComputeBarycentricCoordinates(int px, int py, triangle t, float& alpha, float& beta, float& gamma)
{
	glm::vec4 a = t.v1.pos;
	glm::vec4 b = t.v2.pos;
	glm::vec4 c = t.v3.pos;

	float lineABP = (b[1] - a[1]) * px + (a[0] - b[0]) * py + b[0] * a[1] - a[0] * b[1];
	float lineBCA = (c[1] - b[1]) * a[0] + (b[0] - c[0]) * a[1] + c[0] * b[1] - b[0] * c[1];
	float lineACP = (c[1] - a[1]) * px + (a[0] - c[0]) * py + c[0] * a[1] - a[0] * c[1];
	float lineACB = (c[1] - a[1]) * b[0] + (a[0] - c[0]) * b[1] + c[0] * a[1] - a[0] * c[1];
	float lineBCP = (c[1] - b[1]) * px + (b[0] - c[0]) * py + c[0] * b[1] - b[0] * c[1];
	float lineABC = (b[1] - a[1]) * c[0] + (a[0] - b[0]) * c[1] + b[0] * a[1] - a[0] * b[1];

	alpha = lineBCP / lineBCA;
	beta = lineACP / lineACB;
	gamma = lineABP / lineABC;
}

void ShadeFragment(triangle tri, float& alpha, float& beta, float& gamma, glm::vec3& col, float& depth)
{
	if (0 <= alpha && 0 <= beta && 0 <= gamma)
	{
		if (1 >= alpha && 1 >= beta && 1 >= gamma) {

			//If the furthest point on previous pixel is further than the furthest point on this triangle, render this triangle on top, otherwise, leave
			
			if (min({ tri.v1.pos[2], tri.v2.pos[2], tri.v3.pos[2] }) < depth)//Furthest point of triangle is closer than prev pixel depth
			{
				col = tri.v1.col;
				depth = min({ tri.v1.pos[2], tri.v2.pos[2], tri.v3.pos[2] });
			}
		}
	}
}


void Rasterise(vector<triangle> tris)
{
	float alpha, beta, gamma;
	for (int py = 0; py < PIXEL_H; py++)
	{
		float percf = (float)py / (float)PIXEL_H;
		int perci = percf * 100;
		std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;

		for (int px = 0; px < PIXEL_W; px++)
		{
			for (triangle tri : tris) {
				float cDepth = depth_buffer[py * PIXEL_W + px];
				glm::vec3 col = glm::vec3(1.f);
				ComputeBarycentricCoordinates(px, py, tri, alpha, beta, gamma);
				ShadeFragment(tri, alpha, beta, gamma, col, cDepth);
				if (col == tri.v1.col)
				{
					writeColToDisplayBuffer(col,px,py);
					depth_buffer[py * PIXEL_W + px] = cDepth;
				}
			}
		}
	}
	std::clog << "\rFinish rendering.           \n";
}

void render(vector<triangle>& tris)
{
	float col[] = { 1.f,1.f,1.f,1.f };
	glm::mat4 projMtx = glm::mat4(1.0f);
	glm::mat4 viewMtx = glm::mat4(1.0f);
	glm::mat4 goalMtx = glm::mat4(1.0f);
	glm::mat4 modelMtx = glm::mat4(1.0f);
	//Clear the buffers
	ClearColourBuffer(col);
	ClearDepthBuffer();
	//Apply transformations
	modelMtx = glm::scale(modelMtx, glm::vec3(1.f, -1.f, 1.f));
	viewMtx = glm::translate(viewMtx, glm::vec3(0.1, -2.5, -6.f));
	projMtx = glm::perspective(glm::radians(55.f), (float)PIXEL_W / (float)PIXEL_H, 0.1f, 10.f);
	
	ApplyTransformationMatrix(viewMtx, tris);
	ApplyTransformationMatrix(projMtx, tris);
	ApplyTransformationMatrix(modelMtx, tris);

	ApplyPerspectiveDivision(tris);
	ApplyViewportTransformation(PIXEL_W, PIXEL_H, tris);
	Rasterise(tris);

}
