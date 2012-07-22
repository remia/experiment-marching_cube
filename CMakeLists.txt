cmake_minimum_required(VERSION 2.6)

project(MarchingCube)

set(CMAKE_CXX_FLAGS "-std=c++0x -g")

include_directories(/home/rem/glm/glm-0.9.3.4/)
link_directories()

set(source_files
	src/main.cpp
	src/MyGLWindow.cpp
	src/MarchingCube.cpp
	src/GLSystem/GLContext.cpp
	src/GLSystem/GLShaderProgram.cpp
	src/GLSystem/Glut/GLWindowGlut.cpp
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
)

