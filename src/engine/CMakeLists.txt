#
# CMakeLists.txt
# author: Jan Dušek <jan.dusek90@gmail.com>

include_directories(
	${SDL2_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS}
	${PROJECT_SOURCE_DIR}/src/utils
)

# on MSVC we use static glew and therefore we need to add proper definitions
if (MSVC)
	add_definitions(/D GLEW_STATIC)
endif()

set(SM_ENGINE_HEADERS
	Renderer.h
	Camera.h
	FpsCamera.h
	Common.h
	Buffer.h
	UniformBuffer.h
	Shader.h
	ShaderProgram.h
	ShaderManager.h
	Mesh.h
	Terrain.h
	RenderBatch.h
	VertexArrayObject.h
	Material.h
	Light.h
)

set(SM_ENGINE_SOURCES
	Renderer.cpp
	Camera.cpp
	FpsCamera.cpp
	Shader.cpp
	ShaderProgram.cpp
	ShaderManager.cpp
	Mesh.cpp
	Terrain.cpp
	RenderBatch.cpp
	Light.cpp
)

add_library(engine ${SM_ENGINE_SOURCES} ${SM_ENGINE_HEADERS})
target_link_libraries(engine utils ${SDL2_LIBRARY} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})