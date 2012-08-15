cmake_minimum_required(VERSION 2.6)

project(MarchingCube)

set(CMAKE_CXX_FLAGS "-std=c++0x -fopenmp -O3")

set(SDL_BUILDING_LIBRARY true)
Find_Package(SDL REQUIRED)

include_directories(
	/home/rem/glm/glm-0.9.3.4/
	/home/rem/assimp-3.0/include
)

link_directories(
	/home/rem/assimp-3.0/lib
)

set(source_files
	src/main.cpp
	src/MyGLWindow.cpp
	src/MarchingCube.cpp
	src/Utils/Chrono.cpp
	src/Utils/PerlinNoise.cpp
	src/GLRenderer/Utils.cpp
	src/GLRenderer/Context.cpp
	src/GLRenderer/Camera.cpp
	src/GLRenderer/Texture.cpp
	src/GLRenderer/Mesh.cpp
	src/GLRenderer/Material.cpp
	src/GLRenderer/Actor.cpp
	src/GLRenderer/Entity.cpp
	src/GLRenderer/Scene.cpp
	src/GLRenderer/ShaderProgram.cpp
	src/GLRenderer/ModelLoader.cpp
	src/GLRenderer/Glut/WindowGlut.cpp
	src/GLRenderer/SDL/WindowSDL.cpp
)

add_executable(
	bin/marching_cube
	${source_files}
)

target_link_libraries(
	bin/marching_cube
	glut
	GLU
	GLEW
	${SDL_LIBRARY}
	SDLmain
	assimp
	freeimageplus
)

