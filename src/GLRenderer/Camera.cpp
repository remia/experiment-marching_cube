#include "Camera.h"

namespace GLRenderer
{
	Camera::Camera()
		: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f),
		_acceleration(0.0f, 0.0f, 0.0f)
	{

	}

	Camera::~Camera()
	{

	}

	glm::mat4 Camera::ViewMatrix() const
	{
		// note : camera transformation <=> inverse transformation in world space	
		glm::mat4 viewMatrix = glm::mat4(1.0f);
		viewMatrix = glm::rotate(viewMatrix, - _rotation.x / 16.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, - _rotation.y / 16.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		viewMatrix = glm::rotate(viewMatrix, - _rotation.z / 16.0f, glm::vec3(0.0f, 0.0f, 1.0f));

		viewMatrix = glm::translate(viewMatrix, - _position);	

		return viewMatrix;
	}

	const glm::vec3& Camera::Position() const
	{
		return _position;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		_position = position;
	}

	const glm::vec3& Camera::Rotation() const
	{
		return _rotation;
	}

	void Camera::SetRotation(const glm::vec3& rotation)
	{
		_rotation = rotation;
	}

	void Camera::SetRotationX(const float rotation)
	{
		_rotation.x = rotation;
	}

	void Camera::SetRotationY(const float rotation)
	{
		_rotation.y = rotation;
	}

	void Camera::SetRotationZ(const float rotation)
	{
		_rotation.z = rotation;
	}

	const glm::vec3& Camera::Acceleration() const
	{
		return _acceleration;
	}

	void Camera::SetAcceleration(const glm::vec3& acceleration)
	{
		_acceleration = acceleration;
	}

	void Camera::Move(const float movement)
	{
		glm::vec3 target(0.0f, 0.0f, 1.0f);
		target = glm::rotateX(target, _rotation.x / 16.0f);	
		target = glm::rotateY(target, _rotation.y / 16.0f);	
		target = glm::rotateZ(target, _rotation.z / 16.0f);	

		_position += target * movement;
	}
}
