#pragma once
#include <vector>
#include <cstdint>

#include <glm/glm.hpp>

namespace Atlas {
	struct Vertex {

		glm::vec3 position;
		float uv_x;

		glm::vec3 normal;
		float uv_y;
		
		glm::vec4 color;
	};

	struct GeometryInfo {
		std::vector<Vertex> verticies;
		std::vector<uint32_t> indices;

		size_t vertexCount{ 0 };
		size_t indexCount{ 0 };
		size_t triangleCount{ 0 };

		GeometryInfo(std::vector<Vertex> const& verticies, std::vector<uint32_t> const& indices, size_t vertexCount, size_t indexCount, size_t triangleCount) 
			: verticies(verticies), indices(indices), vertexCount(vertexCount), indexCount(indexCount), triangleCount(triangleCount) {}

		GeometryInfo(std::vector<Vertex> const& verticies, std::vector<uint32_t> const& indices, size_t vertexCount, size_t indexCount)
			: GeometryInfo(verticies, indices, vertexCount, indexCount, indexCount / 3) {}

		GeometryInfo(std::vector<Vertex> const& verticies, std::vector<uint32_t> const& indices)
			: GeometryInfo(verticies, indices, verticies.size(), indices.size()) {}

		GeometryInfo() = default;

		size_t getVertexBufferSize() const { return verticies.size() * sizeof(Vertex); }
		size_t getIndexBufferSize() const { return indices.size() * sizeof(uint32_t); }
	};

	struct GeometryUploadInfo {
		std::vector<Vertex> verticies;
		std::vector<uint32_t> indices;

		size_t vertexCount = 0;
		size_t indexCount = 0;

		GeometryUploadInfo(std::vector<Vertex> const& verticies, std::vector<uint32_t> const& indices, size_t vertexCount, size_t indexCount)
			: verticies(verticies), indices(indices), vertexCount(vertexCount), indexCount(indexCount) {}

		GeometryUploadInfo(std::vector<Vertex> const& verticies, std::vector<uint32_t> const& indices) 
			: GeometryUploadInfo(verticies, indices, verticies.size(), indices.size()) {}

		GeometryUploadInfo() = default;

		size_t getVertexBufferSize() const { return verticies.size() * sizeof(Vertex); }
		size_t getIndexBufferSize() const { return indices.size() * sizeof(uint32_t); }
	};

	struct Mesh {
		GeometryInfo geometryInfo;
		
	};
}
