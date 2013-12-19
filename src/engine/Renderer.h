/**
 * @file Renderer.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "Common.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <queue>

class Camera;
class ShaderManager;
class Terrain;

namespace gl {

class RenderBatch;
class ShaderProgram;
class IndexedBuffer;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initialize();

	void setViewport(const Viewport& viewport);

	/// Sets camera which will be used.
	void setCamera(Camera* camera);

	void setTerrain(Terrain* terrain);

	/// Draw single frame, drawing all registered nodes
	void drawFrame();
	
	ShaderManager* shaderManager();

private:
	void drawBatch(const RenderBatch& batch);
	void drawTerrain();

	static const int CAMERA_BINDING_POINT = 0;
	static const int MATERIAL_BINDING_POINT = 1;

	struct State
	{
		gl::ShaderProgram* shader = nullptr;
		gl::IndexedBuffer* materialUbo = nullptr;
	};

	Viewport m_viewport;

	State m_currentState;
	std::unique_ptr<RenderBatch> m_terrain;
};

}

#endif // !RENDERER_H
