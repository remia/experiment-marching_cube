#pragma once

#include "GLSystem/GLWindowBase.h"
#include "GLSystem/GLShaderProgram.h"
#include "GLSystem/GLPrimitiveLibrary.h"
#include "GLSystem/Entity3D.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>

// Custom class : have to define initialize, resize & paint
class MyGLWindow : public GLWindowBase
{
public:
	MyGLWindow();
	~MyGLWindow();

public:
	// GL default events
	virtual void initialize();
	virtual void resize(int width, int height);
	virtual void paint();

	// Input event
	virtual void mouseEvent(int buttons, int state, int x, int y);
	virtual void mouseMove(int x, int y);
	virtual void keyboardEvent(int keys, int modifiers) {}

private:
	// used by marching cube initialization
	float Potential(const float x, const float y, const float z);

private:
	GLPrimitiveLibrary _primitiveLibrary;
	std::vector<Entity3D> _actors;
	GLShaderProgram sprog;

	glm::i16vec2 lastMousePos;
	float xRotate;
	float yRotate;
	float zRotate;

	glm::mat4 projectionMatrix;
};
