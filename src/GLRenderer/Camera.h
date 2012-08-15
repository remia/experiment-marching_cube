#pragma once

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace GLRenderer
{
	class Camera
	{
		public:
			Camera();
			~Camera();

		public:
			glm::mat4 ViewMatrix() const;

		public:
			const glm::vec3& Position() const;
			void SetPosition(const glm::vec3& position);

			const glm::vec3& Rotation() const;
			void SetRotation(const glm::vec3& rotation);
			void SetRotationX(const float rotation);
			void SetRotationY(const float rotation);
			void SetRotationZ(const float rotation);

			const glm::vec3& Acceleration() const;
			void SetAcceleration(const glm::vec3& acceleration);

			// move camera along target direction
			void Move(const float movement);

		private:
			glm::vec3 _position;
			glm::vec3 _rotation;
			glm::vec3 _acceleration;

	};
}
