#include "Actor.h"

#include "Utils.h"
#include "Mesh.h"
#include "ShaderProgram.h"

namespace GLRenderer
{
	Actor::Actor()
	{

	}

	Actor::~Actor()
	{

	}

	ShaderProgramT& Actor::GetShaderProgram()
	{
		return _shaderProgram;
	}

	void Actor::SetShaderProgram(const ShaderProgramT& shaderProgram)
	{
		_shaderProgram = shaderProgram;
	}

	void Actor::AddDrawableMesh(const MeshT& mesh, const Material& material)
	{
		DrawableMesh dm(mesh, material);
		_dMesh.push_back(dm);
	}

	void Actor::Draw(const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
	{
		_shaderProgram->Install();

		// set matrices model view proj
		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));

		glm::mat4 pmvMatrix = projMatrix * viewMatrix;
		pmvMatrix *= modelMatrix;

		// TODO : add helper function in ShaderProgram
		glUniformMatrix3fv(_shaderProgram->GetUniformLocation("NormalMatrix"),
			1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(_shaderProgram->GetUniformLocation("ModelViewMatrix"),
			1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(_shaderProgram->GetUniformLocation("MVP"),
			1, GL_FALSE, glm::value_ptr(pmvMatrix));

		for(DrawableMesh dm : _dMesh)
		{
			dm.second.PrepareDraw(_shaderProgram);
			dm.first->Draw();
		}

		_shaderProgram->Release();

		Utils::CheckErrorAndPrint("Actor::Draw");
	}
}
