#include "GLCamera.h"

GLCamera::GLCamera()
	: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f),
	_acceleration(0.0f, 0.0f, 0.0f)
{

}

GLCamera::~GLCamera()
{

}

glm::mat4 GLCamera::ViewMatrix() const
{
	// note : camera transformation <=> inverse transformation in world space	

	glm::mat4 viewMatrix = glm::mat4(1.0f);
	viewMatrix = glm::rotate(viewMatrix, - _rotation.x / 16.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, - _rotation.y / 16.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	viewMatrix = glm::rotate(viewMatrix, - _rotation.z / 16.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	viewMatrix = glm::translate(viewMatrix, - _position);	

	return viewMatrix;
}

const glm::vec3& GLCamera::Position() const
{
	return _position;
}

void GLCamera::SetPosition(const glm::vec3& position)
{
	_position = position;
}

const glm::vec3& GLCamera::Rotation() const
{
	return _rotation;
}

void GLCamera::SetRotation(const glm::vec3& rotation)
{
	_rotation = rotation;
}

void GLCamera::SetRotationX(const float rotation)
{
	_rotation.x = rotation;
}

void GLCamera::SetRotationY(const float rotation)
{
	_rotation.y = rotation;
}

void GLCamera::SetRotationZ(const float rotation)
{
	_rotation.z = rotation;
}

const glm::vec3& GLCamera::Acceleration() const
{
	return _acceleration;
}

void GLCamera::SetAcceleration(const glm::vec3& acceleration)
{
	_acceleration = acceleration;
}

void GLCamera::Move(const float movement)
{
	glm::vec3 target(0.0f, 0.0f, 1.0f);
	target = glm::rotateX(target, _rotation.x / 16.0f);	
	target = glm::rotateY(target, _rotation.y / 16.0f);	
	target = glm::rotateZ(target, _rotation.z / 16.0f);	

	_position += target * movement;
}
