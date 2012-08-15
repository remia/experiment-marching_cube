#include "Scene.h"

#include "Actor.h"
#include "ShaderProgram.h"
#include "ShaderLocation.h"
#include "Utils.h"

namespace GLRenderer
{
	Scene::Scene()
		: _uboUpToDate(false), _uboHandle(0)
	{

	}

	Scene::~Scene()
	{
		// TODO : delete ubo
	}

	void Scene::AddEntity(const Entity& e)
	{
		_sceneEntities.push_back(e);
		_uboUpToDate = false;
	}

	void Scene::Init()
	{
		// no need to init if scene is empty
		if(_sceneEntities.empty())
		{
			return;
		}

		ActorT actor = _sceneEntities.front().Actor();
		ShaderProgramT& firstShaderProg = _sceneEntities.front().Actor()->GetShaderProgram();
		GLint sizeUBO = firstShaderProg->GetUniformBlockSize("GlobalMatrices");

		// init ubo
		if(_uboHandle == 0)
		{
			// retrive offset to matrices
			_viewMatrixOffset = firstShaderProg->GetUniformBlockOffset("GlobalMatrices", "ViewMatrix");

			// generate ubo
			glGenBuffers(1, &_uboHandle);
			glBindBuffer(GL_UNIFORM_BUFFER, _uboHandle);
			glBufferData(GL_UNIFORM_BUFFER, sizeUBO, NULL, GL_STREAM_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		Utils::CheckErrorAndPrint("Scene::Init2");
		// bind uniform block to a binding point
		// TODO : redondant, only call for each different shaderPrograms
		for(Entity& e : _sceneEntities)
		{
			ActorT& actor = e.Actor();
			ShaderProgramT sprog = actor->GetShaderProgram();

			sprog->Install();
			sprog->BindUniformBlock("GlobalMatrices", ShaderLocation::GlobalMatricesBindingIndex);

			// TODO : class Light (special entity)
			// TODO : Light in eye coord, compute here not in vertex shader
			struct LightInfo
			{
				glm::vec4 Position;	// Light position in eye coords
				glm::vec3 Intensity;// Light intensity (ads)
			};

			// set light params
			LightInfo Light;
			Light.Position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
			Light.Intensity = glm::vec3(1.0f, 1.0f, 1.0f);
			Light.Position = glm::vec4(0.0f, 8.0f, -10.0f, 1.0f);

			glUniform4fv(sprog->GetUniformLocation("Light.Position"), 1, glm::value_ptr(Light.Position));
			glUniform3fv(sprog->GetUniformLocation("Light.Intensity"), 1, glm::value_ptr(Light.Intensity));

			sprog->Release();
		}

		// attach ubo to binding point
		glBindBufferRange(GL_UNIFORM_BUFFER, ShaderLocation::GlobalMatricesBindingIndex,
			_uboHandle, 0, sizeUBO);

		Utils::CheckErrorAndPrint("Scene::Init");
	}

	void Scene::Update(const float ellapsed)
	{
		for(Entity& e : _sceneEntities)
		{
			e.Update(ellapsed);
		}
	}

	void Scene::Draw()
	{
		if(!_uboUpToDate)
		{
			Init();
		}

		// view space matrix
		glm::mat4 viewMatrix = _camera.ViewMatrix();

		// update matrices through ubo
		glBindBuffer(GL_UNIFORM_BUFFER, _uboHandle);
		glBufferSubData(GL_UNIFORM_BUFFER, _viewMatrixOffset, sizeof(glm::mat4), glm::value_ptr(viewMatrix));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		for(Entity& e : _sceneEntities)
		{
			e.Draw(viewMatrix, _projectionMatrix);
		}
	}

	void Scene::Resize(unsigned int width, unsigned int height)
	{
		if (height == 0) height = 1;
		glViewport (0, 0, (GLsizei) width, (GLsizei) height);

		_projectionMatrix = glm::perspective(45.0f, (1.0f * width) / (1.0f * height), 0.1f, 100.0f);
	}

	Camera& Scene::GetCamera()
	{
		return _camera;
	}
}
