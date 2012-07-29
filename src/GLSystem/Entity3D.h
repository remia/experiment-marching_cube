#pragma once

#include "GLPrimitiveLibrary.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

class Entity3D
{
public:
	Entity3D();
	~Entity3D();

public:
	const glm::vec3& Position();
	void SetPosition(const glm::vec3& position);

	const glm::vec3& Rotation();
	void SetRotation(const glm::vec3& rotation);
	void SetRotationX(const float rotation);
	void SetRotationY(const float rotation);
	void SetRotationZ(const float rotation);

	const glm::vec3& Acceleration();
	void SetAcceleration(const glm::vec3& acceleration);

	void SetPrimitive(GLPrimitiveT& primitive);
	void SetPrimitive(GLPrimitiveLibrary& lib, const IdT ID);

public:
	void Draw();

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _acceleration;
	
	GLPrimitiveT _primitive;
};
