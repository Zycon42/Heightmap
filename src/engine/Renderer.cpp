/**
 * @file Renderer.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Renderer.h"

#include "Camera.h"
#include "Logging.h"
#include "Exception.h"
#include "ShaderManager.h"
#include "Terrain.h"
#include "RenderBatch.h"

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

namespace gl {

Renderer::Renderer() {

}

Renderer::~Renderer() { }

void Renderer::initialize() {
	glEnable(GL_DEPTH_TEST);
}

ShaderManager* Renderer::shaderManager() {
	static ShaderManager manager;
	return &manager;
}

void Renderer::setViewport(const Viewport& viewport) {
	glViewport(static_cast<int>(viewport.x), static_cast<int>(viewport.y), 
		static_cast<size_t>(viewport.width), static_cast<size_t>(viewport.height));
	glDepthRange(viewport.znear, viewport.zfar);

	m_viewport = viewport;
}

void Renderer::drawFrame() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_terrain)
		drawTerrain();
}

void Renderer::setCamera(Camera* camera) {
	camera->uniformBuffer()->bind(CAMERA_BINDING_POINT, GL_UNIFORM_BUFFER);
}

void Renderer::setTerrain(Terrain* terrain) {
	m_terrain = std::make_unique<RenderBatch>();

	auto mesh = terrain->mesh();
	if (!mesh || !mesh->isValid()) {
		LOG(ERROR) << "Renderable with invalid mesh registered to renderer.";
		return;
	}

	// create OpenGL geometry i.e. vao, vbo etc.

	auto vbo = std::make_shared<Buffer>();
	vbo->loadData(mesh->vertexData().data(), mesh->vertexData().size());

	auto geom = new GeometryBatch();
	geom->vao().bind();
	geom->setPrimitiveType(mesh->primitiveType());
	geom->setVertices(vbo, mesh->vertexCount(), mesh->vertexLayout());
	if (mesh->isIndexed()) {
		auto ebo = std::make_shared<Buffer>();
		ebo->loadData(mesh->indices().data(), mesh->indices().size() * sizeof(uint32_t));
		geom->setIndices(ebo, GL_UNSIGNED_INT, mesh->indices().size());
	}
	VertexArrayObject::unbind();
	m_terrain->geometry.reset(geom);
}

void Renderer::drawTerrain() {
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	shaderManager()->getGlslProgram("simple")->use();

	auto geom = m_terrain->geometry.get();
	geom->vao().bind();
	if (geom->hasElements())
		glDrawElements(geom->drawMode(), geom->indexCount(), geom->elementsType(), nullptr);
	else
		glDrawArrays(geom->drawMode(), 0, geom->vertexCount());

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

}
