#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Camera.h"
#include "Entity.h"

#include <vector>

namespace GLRenderer
{
	class Scene
	{
		public:
			Scene();
			~Scene();

			Scene(const Scene& rhs) = delete;
			Scene& operator=(const Scene& rhs) = delete;

		public:
			// TODO : AddLight

			void AddEntity(const Entity& e);

			void Init();

			void Update(const float ellapsed);

			void Draw();

			void Resize(unsigned int width, unsigned int height);

			// allow direct camera manipulation
			Camera& GetCamera();

		private:
			std::vector<Entity> _sceneEntities;

			Camera _camera;
			glm::mat4 _projectionMatrix;

			// ubo
			GLuint _uboHandle;
			bool _uboUpToDate;

			GLint _viewMatrixOffset;
			GLint _projectionMatrixOffset;
	};
}
