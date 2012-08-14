#pragma once

#include "GLPrimitiveLibrary.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <string>

class Entity3D
{
public:
	Entity3D();
	explicit Entity3D(const std::string& id); /* temporary, waiting for scene manager */
	~Entity3D();

public:
	const std::string& GetID() const;

	const glm::vec3& Position() const;
	void SetPosition(const glm::vec3& position);

	const glm::vec3& Rotation() const;
	void SetRotation(const glm::vec3& rotation);
	void SetRotationX(const float rotation);
	void SetRotationY(const float rotation);
	void SetRotationZ(const float rotation);

	const glm::vec3& Acceleration() const;
	void SetAcceleration(const glm::vec3& acceleration);

	const glm::vec3& Scale() const;
	void SetScale(const glm::vec3& scale);

	void SetPrimitive(GLEntityT& primitive);
	void SetPrimitive(GLPrimitiveLibrary& lib, const IdT ID);

public:
	void Draw();

private:
	glm::vec3 _position;
	glm::vec3 _rotation;
	glm::vec3 _acceleration;
	glm::vec3 _scale;
	
	GLEntityT _primitive;

	std::string _id;
};
