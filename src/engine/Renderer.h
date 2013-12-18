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

namespace gl {

class Renderer
{
public:
	Renderer();
	~Renderer();

	void initialize();

	void setViewport(const Viewport& viewport);

	/// Sets camera which will be used.
	void setCamera(Camera* camera);

	/// Draw single frame, drawing all registered nodes
	void drawFrame();
	
	ShaderManager* shaderManager();

private:
	static const int CAMERA_BINDING_POINT = 0;

	Camera* m_camera;
	Viewport m_viewport;
};

}

#endif // !RENDERER_H
