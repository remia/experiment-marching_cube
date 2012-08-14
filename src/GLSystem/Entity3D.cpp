#include "Entity3D.h"

Entity3D::Entity3D()
	: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f),
	_acceleration(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f)
{

}

Entity3D::Entity3D(const std::string& id)
	: Entity3D() 
{
	_id = id;
}

Entity3D::~Entity3D()
{

}

const std::string& Entity3D::GetID() const
{
	return _id;
}

const glm::vec3& Entity3D::Position() const
{
	return _position;
}

void Entity3D::SetPosition(const glm::vec3& position)
{
	_position = position;
}

const glm::vec3& Entity3D::Rotation() const
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

const glm::vec3& Entity3D::Acceleration() const
{
	return _acceleration;
}

void Entity3D::SetAcceleration(const glm::vec3& acceleration)
{
	_acceleration = acceleration;
}

const glm::vec3& Entity3D::Scale() const
{
	return _scale;
}

void Entity3D::SetScale(const glm::vec3& scale)
{
	_scale = scale;
}

void Entity3D::SetPrimitive(GLEntityT& primitive)
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
