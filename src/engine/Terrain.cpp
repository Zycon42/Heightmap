/**
* @file Terrain.cpp
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#include "Terrain.h"

#include "ArrayRef.h"

Terrain::Terrain(const HeightMap& heightMap) {
	std::vector<glm::vec3> vertices;
	for (size_t y = 0; y < heightMap.height; ++y) {
		for (size_t x = 0; x < heightMap.width; ++x) {
			vertices.emplace_back(x, y, heightMap.heights[y * heightMap.width + x]);
		}
	}

	std::vector<VertexElement> layout = { VertexElement(3) };
	m_mesh.loadVertices(ArrayRef<char>((char*)vertices.data(), vertices.size() * sizeof(glm::vec3)),
		vertices.size(), layout);

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
