#pragma once
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
class Vertex {
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Face {
public:
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 v3;
};

void obj_parse(std::vector<Vertex>& vertexArray, std::vector<Face>& faceArray, const char* filename) {
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
		for (int i = 0; i < shape.mesh.indices.size()-2; i++) {
			Face face{};
			if (strcmp(filename, "objs/bumpy_plane.obj") == 0) {
				face.v1 = {
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index + 1] - 4,
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index + 2],
				};
				face.v2 = {
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index + 1] - 4,
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index + 2],
				};
				face.v3 = {
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index + 1] - 4,
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index + 2],
				};
			}
			else if (strcmp(filename, "objs/flat_bumpy_plane.obj") == 0) {
				face.v1 = {
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index + 1] +3.5,
					attribs.vertices[3 * shape.mesh.indices[i].vertex_index + 2],
				};
				face.v2 = {
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index + 1] +3.5,
					attribs.vertices[3 * shape.mesh.indices[i + 1].vertex_index + 2],
				};
				face.v3 = {
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index],
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index + 1] +3.5,
					attribs.vertices[3 * shape.mesh.indices[i + 2].vertex_index + 2],
				};
			}
			faceArray.push_back(face);
		}
		for (const auto& index : shape.mesh.indices)
		{
			Vertex vert{};
			
			if (strcmp(filename, "objs/bumpy_plane.obj") == 0) {
				vert.position =
				{
					attribs.vertices[3 * index.vertex_index],
					attribs.vertices[3 * index.vertex_index + 1] - 4,
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
			}
			else if (strcmp(filename, "objs/flat_bumpy_plane.obj") == 0) {
				vert.position =
				{
					attribs.vertices[3 * index.vertex_index],
					attribs.vertices[3 * index.vertex_index + 1] + 3.5,
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
			}
			//vert.transparency = glm::vec1(1.f);

			vertexArray.push_back(vert);
		}
	}
}