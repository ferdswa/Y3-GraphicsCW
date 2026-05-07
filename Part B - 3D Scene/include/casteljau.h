#pragma once
#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::list<point> P)
{
	list<point> Q = P;
	while (Q.size()>1) {
		list<point> R;

		for (auto p1 = Q.begin(); p1 != Q.end(); ++p1) {
			auto p2 = next(p1);
			if (p2 != Q.end()) {
				point p;
				p.x = ((1 - t) * p1->x) + (t * p2->x);
				p.y = ((1 - t) * p1->y) + (t * p2->y);
				p.z = ((1 - t) * p1->z) + (t * p2->z);
				R.push_back(p);
			}
		}
		Q.clear();
		Q = R;
	}
	return Q.front();
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	std::list<point> ps(ctrl_points.begin(), ctrl_points.end());
	std::vector<point> curve;

	float offset = 1.f / num_evaluations;
	curve.push_back(ctrl_points[0]);

	for (int i = 0; i < num_evaluations - 1; i++) {
		point p = evaluate(offset * (i + 1), ps);
		curve.push_back(p);
	}

	return curve;
}

float* MakeFloatsFromVector(std::vector<point> curve, int& num_verts, int& num_floats, float r, float g, float b)
{
	num_verts = curve.size();
	if (num_verts == 0)
		return NULL;
	num_floats = num_verts * 6;
	float* vertices = (float*)malloc(num_floats * sizeof(float));
	int arrpos = 0;
	for (int i = 0; i < curve.size(); i++) {
		vertices[arrpos] = curve[i].x;
		vertices[arrpos + 1] = curve[i].y;
		vertices[arrpos + 2] = curve[i].z;

		vertices[arrpos + 3] = r;
		vertices[arrpos + 4] = g;
		vertices[arrpos + 5] = b;
		arrpos += 6;
	}
	return vertices;
}
