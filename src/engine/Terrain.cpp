/**
* @file Terrain.cpp
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#include "Terrain.h"

#include "ArrayRef.h"

#include <cassert>

struct Vertex {
	glm::vec3 pos, normal;
};

Terrain::Terrain(const HeightMap& heightMap) {
	// check that sizes match to avoid buffer overflow
	assert((heightMap.height * heightMap.width) == heightMap.heights.size());

	auto vertices = computeVertices(heightMap);

	std::vector<VertexElement> layout = { VertexElement(3), VertexElement(3) };
	m_mesh.loadVertices(vertices, vertices.size() / sizeof(Vertex), layout);

	m_mesh.loadIndices(computeIndices(heightMap));
	m_mesh.setPrimitiveType(PrimitiveType::TriangleStrip);
}

std::vector<uint32_t> Terrain::computeIndices(const HeightMap& map) {
	size_t numIndices = (map.width * 2) * (map.height - 1) + (map.height - 2);
	std::vector<uint32_t> result;
	result.resize(numIndices);

	size_t index = 0;
	for (size_t y = 0; y < map.height - 1; ++y) {
		// Even rows move left to right, odd rows move right to left.
		if (y % 2 == 0) {
			size_t x;
			for (x = 0; x < map.width; ++x) {
				result[index++] = x + (y * map.width);
				result[index++] = x + (y * map.width) + map.width;
			}
			// Insert degenerate vertex if this isn't the last row
			if (y != map.height - 2) {
				result[index++] = --x + (y * map.width);
			}
		} else {
			int x;
			for (x = map.width - 1; x >= 0; --x) {
				result[index++] = x + (y * map.width);
				result[index++] = x + (y * map.width) + map.width;
			}
			// Insert degenerate vertex if this isn't the last row
			if (y != map.height - 2) {
				result[index++] = ++x + (y * map.width);
			}
		}
	}

	return result;
}

template <typename T>
class ArrayMapper
{
public:
	ArrayMapper(void* arr, size_t pitch) : m_arr(arr), m_pitch(pitch) { }

	T& operator()(size_t x, size_t y) {
		char* arr = reinterpret_cast<char*>(m_arr);
		return *reinterpret_cast<T*>(arr + ((y * m_pitch + x) * sizeof(T)));
	}
private:
	void* m_arr;
	size_t m_pitch;
};

std::vector<char> Terrain::computeVertices(const HeightMap& map) {
	std::vector<char> storage;
	// allocate vertices storage
	storage.resize(sizeof(Vertex) * map.heights.size());

	ArrayMapper<Vertex> vertices{ storage.data(), map.width };
	for (size_t y = 0; y < map.height; ++y) {
		for (size_t x = 0; x < map.width; ++x) {
			vertices(x, y).pos = glm::vec3(x, y, map.heights[y * map.width + x]);

			float z0 = vertices(x, y).pos.z;
			float az = (x + 1 < map.width) ? (vertices(x + 1, y).pos.z) : z0;
			float bz = (y + 1 < map.height) ? (vertices(x, y + 1).pos.z) : z0;
			float cz = (x > 0) ? (vertices(x - 1, y).pos.z) : z0;
			float dz = (y > 0) ? (vertices(x, y - 1).pos.z) : z0;
			vertices(x, y).normal = glm::normalize(glm::vec3(cz - az, dz - bz, 2.0f));
		}
	}

	return storage;
}
