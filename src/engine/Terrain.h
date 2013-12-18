/**
* @file Terrain.h
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#ifndef TERRAIN_H
#define TERRAIN_H

#include "Mesh.h"

#include <glm/glm.hpp>

struct HeightMap
{
	size_t width, height;
	std::vector<float> heights;
};

class Terrain
{
public:
	explicit Terrain(const HeightMap& heightMap);

	Mesh* mesh() {
		return &m_mesh;
	}
private:
	static std::vector<uint32_t> computeIndices(const HeightMap& heightMap);

	Mesh m_mesh;
};

#endif // !TERRAIN_H
