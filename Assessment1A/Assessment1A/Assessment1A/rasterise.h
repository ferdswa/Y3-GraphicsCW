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


		tri.v1.nor.x = tri.v1.pos.x;
		tri.v1.nor.y = tri.v1.pos.y;
		tri.v1.nor.z = tri.v1.pos.z;
		tri.v2.nor.x = tri.v2.pos.x;
		tri.v2.nor.y = tri.v2.pos.y;
		tri.v2.nor.z = tri.v2.pos.z;
		tri.v3.nor.x = tri.v3.pos.x;
		tri.v3.nor.y = tri.v3.pos.y;
		tri.v3.nor.z = tri.v3.pos.z;
	}
}

void ApplyPerspectiveDivision(vector<triangle>& tris)
{
	int w = 0;
	for (triangle& tri : tris) {
		//v1
		w = tri.v1.pos.w;
		tri.v1.pos.x = tri.v1.pos.x / w;
		tri.v1.pos.y = tri.v1.pos.y / w;
		tri.v1.pos.z = tri.v1.pos.z / w;
		tri.v1.pos.w = tri.v1.pos.w / w;
		//v2
		w = tri.v2.pos.w;
		tri.v2.pos.x = tri.v2.pos.x / w;
		tri.v2.pos.y = tri.v2.pos.y / w;
		tri.v2.pos.z = tri.v2.pos.z / w;
		tri.v2.pos.w = tri.v2.pos.w / w;
		//v3
		w = tri.v3.pos.w;
		tri.v3.pos.x = tri.v3.pos.x / w;
		tri.v3.pos.y = tri.v3.pos.y / w;
		tri.v3.pos.z = tri.v3.pos.z / w;
		tri.v3.pos.w = tri.v3.pos.w / w;

		tri.v1.nor.x = tri.v1.pos.x;
		tri.v1.nor.y = tri.v1.pos.y;
		tri.v1.nor.z = tri.v1.pos.z;
		tri.v2.nor.x = tri.v2.pos.x;
		tri.v2.nor.y = tri.v2.pos.y;
		tri.v2.nor.z = tri.v2.pos.z;
		tri.v3.nor.x = tri.v3.pos.x;
		tri.v3.nor.y = tri.v3.pos.y;
		tri.v3.nor.z = tri.v3.pos.z;
	}
}

void ApplyViewportTransformation(int w, int h, vector<triangle>& tris)
{
	for (triangle& tri:tris) {
		//v1
		tri.v1.pos.x = (tri.v1.pos.x + 1) * (w/2.f);
		tri.v1.pos.y = (tri.v1.pos.y + 1) * (h/2.f);
		//v2
		tri.v2.pos.x = (tri.v2.pos.x + 1) * (w / 2.f);
		tri.v2.pos.y = (tri.v2.pos.y + 1) * (h / 2.f);
		//v3
		tri.v3.pos.x = (tri.v3.pos.x + 1) * (w / 2.f);
		tri.v3.pos.y = (tri.v3.pos.y + 1) * (h / 2.f);

		tri.v1.nor.x = tri.v1.pos.x;
		tri.v1.nor.y = tri.v1.pos.y;
		tri.v1.nor.z = tri.v1.pos.z;
		tri.v2.nor.x = tri.v2.pos.x;
		tri.v2.nor.y = tri.v2.pos.y;
		tri.v2.nor.z = tri.v2.pos.z;
		tri.v3.nor.x = tri.v3.pos.x;
		tri.v3.nor.y = tri.v3.pos.y;
		tri.v3.nor.z = tri.v3.pos.z;
	}
}


void ComputeBarycentricCoordinates(int px, int py, triangle t, float& alpha, float& beta, float& gamma)
{
	glm::vec4 a = t.v1.pos;
	glm::vec4 b = t.v2.pos;
	glm::vec4 c = t.v3.pos;

	float lineABP = (b.y - a.y) * px + (a.x - b.x) * py + b.x * a.y - a.x * b.y;
	float lineBCA = (c.y - b.y) * a.x + (b.x - c.x) * a.y + c.x * b.y - b.x * c.y;
	float lineACP = (c.y - a.y) * px + (a.x - c.x) * py + c.x * a.y - a.x * c.y;
	float lineACB = (c.y - a.y) * b.x + (a.x - c.x) * b.y + c.x * a.y - a.x * c.y;
	float lineBCP = (c.y - b.y) * px + (b.x - c.x) * py + c.x * b.y - b.x * c.y;
	float lineABC = (b.y - a.y) * c.x + (a.x - b.x) * c.y + b.x * a.y - a.x * b.y;

	alpha = lineBCP / lineBCA;
	beta = lineACP / lineACB;
	gamma = lineABP / lineABC;
}

void ShadeFragment(triangle tri, float& alpha, float& beta, float& gamma, glm::vec3& col, float& depth)
{
	float z1, z2, z3;
	if (0 <= alpha && 0 <= beta && 0 <= gamma)
	{
		if (1 >= alpha && 1 >= beta && 1 >= gamma) {

			//If the furthest point on previous pixel is further than the furthest point on this triangle, render this triangle on top, otherwise, leave
			

			z1 = tri.v1.pos.z;
			z2 = tri.v2.pos.z;
			z3 = tri.v3.pos.z;

			//if (z1 < depth || z2 < depth || z3 < depth) {
			//	col = tri.v1.col;
			//	depth = max({ tri.v1.pos.z, tri.v2.pos.z, tri.v3.pos.z });
			//}
			
			//TODO: IF YOU DONT DO IT TONIGHT, LOOK AT WIKIPEDIA BARYCENTRIC COORDINATES! FORMULA THERE

			if (min({ z1,z2,z3 }) < depth)//Closest point of triangle is closer than prev pixel depth
			{
					col = tri.v1.col;
					depth = min({ tri.v1.pos.z, tri.v2.pos.z, tri.v3.pos.z });
			}
			//if furthest closer -> continue to render, new depth = depth of furthest
			//if closest closer -> continue to render, new depth = depth of closest
		}
	}
}


void Rasterise(vector<triangle> tris)
{
	float alpha, beta, gamma, cDepth;
	
	for (int py = 0; py < PIXEL_H; py++)
	{
		float percf = (float)py / (float)PIXEL_H;
		int perci = percf * 100;
		std::clog << "\rScanlines done: " << perci << "%" << ' ' << std::flush;

		for (int px = 0; px < PIXEL_W; px++)
		{
			glm::vec3 col = glm::vec3(1.f);
			for (triangle tri : tris) {
				cDepth = depth_buffer[py * PIXEL_W + px];
				
				ComputeBarycentricCoordinates(px, py, tri, alpha, beta, gamma);
				ShadeFragment(tri, alpha, beta, gamma, col, cDepth);
				if (col == tri.v1.col)
				{
					depth_buffer[py * PIXEL_W + px] = cDepth;
				}
			}
			writeColToDisplayBuffer(col, px, py);
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
	viewMtx = glm::translate(viewMtx, glm::vec3(0.1, -2.5, -6));
	projMtx = glm::perspective(glm::radians(55.f), (float)PIXEL_W / (float)PIXEL_H, 0.1f, 10.f);
	
	ApplyTransformationMatrix(viewMtx, tris);
	ApplyTransformationMatrix(projMtx, tris);
	ApplyTransformationMatrix(modelMtx, tris);

	ApplyPerspectiveDivision(tris);
	ApplyViewportTransformation(PIXEL_W, PIXEL_H, tris);
	Rasterise(tris);

}
