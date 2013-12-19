/**
* @file Terrain.h
*
* @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
* @date 2013
*/

#ifndef TERRAIN_H
#define TERRAIN_H

#include "Mesh.h"
#include "Material.h"

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

	IMaterial* material() {
		return m_material.get();
	}

	void setMaterial(std::shared_ptr<IMaterial> material) {
		m_material = std::move(material);
	}
private:
	static std::vector<uint32_t> computeIndices(const HeightMap& map);
	static std::vector<char> computeVertices(const HeightMap& map);

	Mesh m_mesh;
	std::shared_ptr<IMaterial> m_material;
};

#endif // !TERRAIN_H
