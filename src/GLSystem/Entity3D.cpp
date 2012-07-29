#include "Entity3D.h"

Entity3D::Entity3D()
	: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f),
	_acceleration(0.0f, 0.0f, 0.0f)
{

}

Entity3D::~Entity3D()
{

}

const glm::vec3& Entity3D::Position()
{
	return _position;
}

void Entity3D::SetPosition(const glm::vec3& position)
{
	_position = position;
}

const glm::vec3& Entity3D::Rotation()
{
	return _rotation;
}

void Entity3D::SetRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
}

void Entity3D::SetRotationX(const float rotation)
{
	_rotation.x = rotation;
}

void Entity3D::SetRotationY(const float rotation)
{
	_rotation.y = rotation;
}

void Entity3D::SetRotationZ(const float rotation)
{
	_rotation.z = rotation;
}

const glm::vec3& Entity3D::Acceleration()
{
	return _acceleration;
}

void Entity3D::SetAcceleration(const glm::vec3& acceleration)
{
	_acceleration = acceleration;
}

void Entity3D::SetPrimitive(GLPrimitiveT& primitive)
{
	_primitive = primitive;
}

void Entity3D::SetPrimitive(GLPrimitiveLibrary& lib, const IdT ID)
{
	_primitive = lib[ID];
}

void Entity3D::Draw()
{
	// Set Shaders params...

	// Draw
	_primitive->Draw();
}
