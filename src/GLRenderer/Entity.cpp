#include "Entity.h"
#include "Actor.h"

namespace GLRenderer
{
	Entity::Entity()
		: _position(0.0f, 0.0f, 0.0f), _rotation(0.0f, 0.0f, 0.0f),
		_acceleration(0.0f, 0.0f, 0.0f), _scale(1.0f, 1.0f, 1.0f)
	{

	}

	Entity::~Entity()
	{

	}

	const glm::vec3& Entity::Position() const
	{
		return _position;
	}

	void Entity::SetPosition(const glm::vec3& position)
	{
		_position = position;
	}

	const glm::vec3& Entity::Rotation() const
	{
		return _rotation;
	}

	void Entity::SetRotation(const glm::vec3& rotation)
	{
		_rotation = rotation;
	}

	void Entity::SetRotationX(const float rotation)
	{
		_rotation.x = rotation;
	}

	void Entity::SetRotationY(const float rotation)
	{
		_rotation.y = rotation;
	}

	void Entity::SetRotationZ(const float rotation)
	{
		_rotation.z = rotation;
	}

	const glm::vec3& Entity::Acceleration() const
	{
		return _acceleration;
	}

	void Entity::SetAcceleration(const glm::vec3& acceleration)
	{
		_acceleration = acceleration;
	}

	const glm::vec3& Entity::Scale() const
	{
		return _scale;
	}

	void Entity::SetScale(const glm::vec3& scale)
	{
		_scale = scale;
	}

	void Entity::SetActor(ActorT& actor)
	{
		_actor = actor;
	}

	ActorT& Entity::Actor()
	{
		return _actor;
	}

	void Entity::Update(const float ellapsed)
	{
		// TODO : implement
	}

	void Entity::Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
	{
		// set up model matrix
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), _position);
		modelMatrix = glm::scale(modelMatrix, _scale);
		// TODO : rotation

		_actor->Draw(modelMatrix, viewMatrix, projMatrix);
	}
}
