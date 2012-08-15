#include "Material.h"

#include "Utils.h"
#include "ShaderProgram.h"

#include <glm/gtc/type_ptr.hpp>

namespace GLRenderer
{

	const char* Material::ShadingModeStr[] = { "ShadingSolid", "ShadingDiffuse", "ShadingADS" };

	Material::Material()
		: _ambientColor(0.0f, 0.0f, 0.0f), _diffuseColor(0.0f, 0.0f, 0.0f),
		_specularColor(0.0f, 0.0f, 0.0f), _specularShininess(1.0f),
		_shadingMode(SHADING_ADS)
	{

	}

	Material::~Material()
	{

	}

	void Material::PrepareDraw(ShaderProgramT& sprog)
	{
		// bind material params
		glUniform3fv(sprog->GetUniformLocation("Material.Ka"), 1, glm::value_ptr(_ambientColor));
		glUniform3fv(sprog->GetUniformLocation("Material.Kd"), 1, glm::value_ptr(_diffuseColor));
		glUniform3fv(sprog->GetUniformLocation("Material.Ks"), 1, glm::value_ptr(_specularColor));
		glUniform1f(sprog->GetUniformLocation("Material.Shininess"), _specularShininess);

		// bind textures
		// TODO : only 1 texture supported atm
		GLuint colorModelLocation = sprog->GetSubroutineIndex(GL_FRAGMENT_SHADER, "ShadeColor");

		for(TextureT& t : _textures)
		{
			colorModelLocation = sprog->GetSubroutineIndex(GL_FRAGMENT_SHADER, "ShadeTexColor");
			glUniform1i(sprog->GetUniformLocation("Tex1"), 0);
		}

		// set fragment shading model
		if(_shadingMode == SHADING_SOLID)
		{
			colorModelLocation = sprog->GetSubroutineIndex(GL_FRAGMENT_SHADER, "PlainColor");
		}

		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorModelLocation);

		// set vertex shading model
		const char* shadingStr = ShadingModeStr[static_cast<int>(_shadingMode)];
		GLuint shadingLocation = sprog->GetSubroutineIndex(GL_VERTEX_SHADER, shadingStr);
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &shadingLocation);

		// check errors
		Utils::CheckErrorAndPrint("Material::PrepareDraw");
	}

	const glm::vec3& Material::AmbientColor() const
	{
		return _ambientColor;
	}

	void Material::SetAmbientColor(const glm::vec3& color)
	{
		_ambientColor = color;
	}

	const glm::vec3& Material::DiffuseColor() const
	{
		return _diffuseColor;
	}

	void Material::SetDiffuseColor(const glm::vec3& color)
	{
		_diffuseColor = color;
	}

	const glm::vec3& Material::SpecularColor() const
	{
		return _specularColor;
	}

	void Material::SetSpecularColor(const glm::vec3& color)
	{
		_specularColor = color;
	}

	float Material::SpecularShininess() const
	{
		return _specularShininess;
	}

	void Material::SetSpecularShininess(const float shininess)
	{
		_specularShininess = shininess;
	}

	Material::ShadingModeT Material::ShadingMode() const
	{
		return _shadingMode;
	}

	void Material::SetShadingMode(const Material::ShadingModeT mode)
	{
		_shadingMode = mode;
	}

	void Material::AddTexture(const TextureT& texture)
	{
		_textures.push_back(texture);
	}
}
