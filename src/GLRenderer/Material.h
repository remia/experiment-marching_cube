#pragma once

#include "AssetLib.h"

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>

#include <vector>

namespace GLRenderer
{
	class Material
	{
		public:
			Material();
			~Material();

		public:
			const glm::vec3& AmbientColor() const;
			void SetAmbientColor(const glm::vec3& color);

			const glm::vec3& DiffuseColor() const;
			void SetDiffuseColor(const glm::vec3& color);

			const glm::vec3& SpecularColor() const;
			void SetSpecularColor(const glm::vec3& color);

			float SpecularShininess() const;
			void SetSpecularShininess(const float shininess);

			enum ShadingModeT { SHADING_SOLID, SHADING_DIFFUSE, SHADING_ADS };
			static const char* ShadingModeStr[3];

			ShadingModeT ShadingMode() const;
			void SetShadingMode(const ShadingModeT mode);

			void AddTexture(const TextureT& texture);

			void PrepareDraw(ShaderProgramT& sprog);

		private:
			glm::vec3 _ambientColor;
			glm::vec3 _diffuseColor;
			glm::vec3 _specularColor;
			float _specularShininess;

			ShadingModeT _shadingMode;

			std::vector<TextureT> _textures;
	};
}
