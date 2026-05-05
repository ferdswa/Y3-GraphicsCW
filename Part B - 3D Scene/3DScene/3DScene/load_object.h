#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

void obj_parse(std::vector<Vertex>& vertexArray, const char* filename) {
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	if (!tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, filename))
	{
		throw std::runtime_error(warn + err);
	}
	for (const auto& shape : shapes)
	{
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vert{};

			vert.position =
			{
				attribs.vertices[3 * index.vertex_index],
				attribs.vertices[3 * index.vertex_index + 1] - 8,
				attribs.vertices[3 * index.vertex_index + 2],
			};

			vert.normal =
			{
				attribs.normals[3 * index.normal_index],
				attribs.normals[3 * index.normal_index + 1],
				attribs.normals[3 * index.normal_index + 2]
			};

			vert.texCoord = {
				attribs.texcoords[index.texcoord_index * 2],
				attribs.texcoords[index.texcoord_index * 2 + 1],
			};

			vertexArray.push_back(vert);
		}
	}
}