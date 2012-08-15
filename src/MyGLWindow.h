#pragma once

#include "GLRenderer/WindowBase.h"
#include "GLRenderer/AssetLib.h"
#include "GLRenderer/Scene.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>

// Custom class : have to define initialize, resize & paint
class MyGLWindow : public GLRenderer::WindowBase
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
	virtual void keyboardEvent(unsigned char keys, int x, int y);

private:
	void InitMCMesh();
	void InitAxis();
	void InitFloor();
	void InitModels();

	// used by marching cube initialization
	float Potential(const float x, const float y, const float z);

private:
	GLRenderer::ActorLib _actorLib;
	GLRenderer::MeshLib _meshLib;
	GLRenderer::TextureLib _textureLib;
	GLRenderer::ShaderProgramLib _shaderProgLib;

	GLRenderer::Scene _scene;

	glm::i16vec2 lastMousePos;
};
