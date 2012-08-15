#pragma once

#include "AssetLib.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

namespace GLRenderer
{
	class Entity
	{
		public:
			Entity();
			~Entity();

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

			void SetActor(ActorT& actor);
			ActorT& Actor();

		public:
			void Update(const float ellapsed);
			void Draw(const glm::mat4& viewMatrix, const glm::mat4& projMatrix);

		private:
			glm::vec3 _position;
			glm::vec3 _rotation;
			glm::vec3 _acceleration;
			glm::vec3 _scale;

			ActorT _actor;
	};
}
