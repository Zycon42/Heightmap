/**
 * @file Application.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Application.h"

#include "Logging.h"
#include "Renderer.h"
#include "FpsCamera.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>

#include <sstream>

const char* SDLApplication::DEFAULT_WND_TITLE = "Test app";

SDLApplication::SDLApplication(int argc, char** argv) 
	: m_window(nullptr), m_context(nullptr), done(false), fps(60.0), windowTitle(DEFAULT_WND_TITLE),
	  m_renderer(new gl::Renderer())
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw SDLException("SDL_Init failed");

	createWindow(windowTitle, 800, 600);
}

SDLApplication::~SDLApplication() {
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

int SDLApplication::run() {
	init();

	uint64_t frameCount = 0;
	double prevTime = getTime();
	while (!done) {
		processEvents();
		update();
		draw();
		calculateFps(fps, prevTime, frameCount);
	}
	
	return 0;
}

void SDLApplication::setWindowTitle(const char* title) {
	if (title == nullptr)
		windowTitle = DEFAULT_WND_TITLE;
	else
		windowTitle = title;

	SDL_SetWindowTitle(m_window, windowTitle);
}

void SDLApplication::createWindow(const char* windowCaption, size_t width, size_t height) {
	this->width = width;
	this->height = height;

	// Set double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create window
	m_window = SDL_CreateWindow(windowCaption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL);
	if (m_window == nullptr)
		throw SDLException("SDL_CreateWindow failed");

	m_context = SDL_GL_CreateContext(m_window);

	LOG(INFO) << "OpenGL version: " << glGetString(GL_VERSION);

	// Init GLEW
	GLenum err;
	glewExperimental = GL_TRUE;
	if ((err = glewInit()) != GLEW_OK)
		throw Exception((const char*)glewGetErrorString(err));

	LOG(INFO) << "Using GLEW " << glewGetString(GLEW_VERSION);

	// GL_EXT_direct_state_access detection broken in glew on my amd radeon 6630m
	// TODO: check if desired functions are not null manually
	/*if (!GLEW_EXT_direct_state_access)
		throw Exception("Missing EXT_direct_state_access extension!");*/

	grabMouse(true);
}

void SDLApplication::init() {
	m_renderer->initialize();
	m_renderer->setViewport(Viewport(static_cast<float>(width), static_cast<float>(height)));

	m_camera = std::unique_ptr<FpsCamera>(new FpsCamera(m_renderer.get()));
	m_camera->setProjectionMatrix(glm::perspective(60.0f, (float)width / height, 0.1f, 1000.0f));
	m_camera->setPosition(0.0f, 0.0f, 250.0f);
	m_camera->setMovementSpeed(60.0f);
	
	m_renderer->setCamera(m_camera.get());
}

void SDLApplication::update() {
	std::ostringstream ss;
	ss << windowTitle << " - " << fps;
	SDL_SetWindowTitle(m_window, ss.str().c_str());

	handleKeyboard();

	m_camera->update();
}

void SDLApplication::draw() {
	
	m_renderer->drawFrame();

	SDL_GL_SwapWindow(m_window);
}

void SDLApplication::processEvents() {
	SDL_Event event;  
	while(SDL_PollEvent(&event)) {
		switch (event.type)  
		{  
		case SDL_QUIT:
			done = true; 
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:
			m_keyboardHandler.handleEvent(event.key);
			break;

		case SDL_MOUSEMOTION:
			handleMouseMove(event.motion.xrel, event.motion.yrel);
			break;
		}  
	}
}

void SDLApplication::handleKeyboard() {
	if (m_keyboardHandler.isPressed(SDLK_ESCAPE)) {
		SDL_Event quitEvent = { SDL_QUIT };
		SDL_PushEvent(&quitEvent);
		return;
	}

	if (m_keyboardHandler.isPressedOnce(SDLK_LALT)) {
		grabMouse(!mouseGrabbed);
	}

	if (m_keyboardHandler.isPressed(SDLK_w))
		m_camera->goForward(fps);
	if (m_keyboardHandler.isPressed(SDLK_s))
		m_camera->goBackward(fps);
	if (m_keyboardHandler.isPressed(SDLK_a))
		m_camera->goLeft(fps);
	if (m_keyboardHandler.isPressed(SDLK_d))
		m_camera->goRight(fps);

	static const float rollSpeed = 45.0f;
	if (m_keyboardHandler.isPressed(SDLK_q))
		m_camera->roll(-rollSpeed / fps);
	if (m_keyboardHandler.isPressed(SDLK_e))
		m_camera->roll(rollSpeed / fps);
}

void SDLApplication::handleMouseMove(int xrel, int yrel) {
	m_camera->yaw(static_cast<float>(xrel));
	m_camera->pitch(static_cast<float>(yrel));
}

void SDLApplication::grabMouse(bool flag) {
	if (SDL_SetRelativeMouseMode(flag ? SDL_TRUE : SDL_FALSE) == -1) {
		LOG(ERROR) << "SDL_SetRelativeMouseMode failed: " << SDL_GetError();
		mouseGrabbed = false;
		return;
	}
	mouseGrabbed = flag;
}

void SDLApplication::calculateFps(float& fps, double& prevTime, uint64_t& frameCount) {
	static bool first = true;

	frameCount++;

	double currentTime = getTime();
	double timeInterval = currentTime - prevTime;
	if (timeInterval > 1.0 || first) {
		fps = static_cast<float>(frameCount / timeInterval);
		prevTime = currentTime;
		frameCount = 0;
		first = false;
	}
}

double SDLApplication::getTime() {
	static uint64_t freq;
	static bool first = true;
	if (first) {
		first = false;
		freq = SDL_GetPerformanceFrequency();
	}

	uint64_t counter = SDL_GetPerformanceCounter();
	return static_cast<double>(counter) / static_cast<double>(freq);
}
