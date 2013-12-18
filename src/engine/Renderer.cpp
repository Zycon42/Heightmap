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
}

void Renderer::setCamera(Camera* camera) {
	camera->uniformBuffer()->bind(CAMERA_BINDING_POINT, GL_UNIFORM_BUFFER);
	m_camera = camera;
}

}
