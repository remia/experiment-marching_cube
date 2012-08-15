#pragma once

#include "AssetLib.h"

#include <string>

// TODO : hide implementation details
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

namespace GLRenderer
{
	class ModelLoader
	{
		public:
			ModelLoader();
			~ModelLoader();

			ModelLoader(const ModelLoader& rhs) = delete;
			ModelLoader& operator=(const ModelLoader& rhs) = delete;

		public:
			ActorT LoadFromFile(const std::string& filename);

		private:
			void InitScene(const aiScene* pScene);
			void InitNode(const aiNode* pNode, const aiScene* pScene, const aiMatrix4x4& transform);
			void InitMesh(const aiMesh* pMesh, const aiMatrix4x4& transform);

		private:
			ActorT _actor;
	};
}
