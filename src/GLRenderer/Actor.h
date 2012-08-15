#pragma once

#include "AssetLib.h"
#include "Material.h"

#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_precision.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <utility>
#include <vector>

namespace GLRenderer
{
	class Actor
	{
		public:
			Actor();
			~Actor();

		public:
			ShaderProgramT& GetShaderProgram();
			void SetShaderProgram(const ShaderProgramT& shaderProgram);

			void AddDrawableMesh(const MeshT& mesh, const Material& material);

			void Draw(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);

		private:
			typedef std::pair<MeshT, Material> DrawableMesh;
			std::vector<DrawableMesh> _dMesh;

			ShaderProgramT _shaderProgram;
	};
}
