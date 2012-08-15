#include "ModelLoader.h"

#include "ShaderLocation.h"
#include "Actor.h"
#include "Mesh.h"
#include "Material.h"
#include "Utils.h"

#include <iostream>

namespace GLRenderer
{
	ModelLoader::ModelLoader()
	{

	}

	ModelLoader::~ModelLoader()
	{

	}

	ActorT ModelLoader::LoadFromFile(const std::string& filepath)
	{
		_actor.reset(new Actor);

		Assimp::Importer importer;
		const aiScene* pScene = importer.ReadFile(filepath, 
				aiProcess_Triangulate |
				aiProcess_GenSmoothNormals |
				aiProcess_SortByPType);

		if(pScene != NULL)
		{
			InitScene(pScene);
		}
		else
		{
			std::cerr << "Can't load model : " << filepath << std::endl;
			std::cerr << importer.GetErrorString() << std::endl;
		}

		return _actor;
	}

	void ModelLoader::InitScene(const aiScene* pScene)
	{
		InitNode(pScene->mRootNode, pScene, aiMatrix4x4());
	}

	void ModelLoader::InitNode(const aiNode* pNode, const aiScene* pScene, const aiMatrix4x4& transform)
	{
		// node transformation & relative to parent transform
		aiMatrix4x4 nodeTransform = pNode->mTransformation;
		aiMatrix4x4 relativeTransform = transform * nodeTransform;

		// init node's mesh
		const unsigned int numMeshs = pNode->mNumMeshes;
		for(unsigned int i = 0; i < numMeshs; ++i)
		{
			const aiMesh* pAiMesh = pScene->mMeshes[ pNode->mMeshes[i] ];
			InitMesh(pAiMesh, relativeTransform);
		}

		// init childrens (recursives call)
		const unsigned int numChilds = pNode->mNumChildren;
		for(unsigned int i = 0; i < numChilds; ++i)
		{
			const aiNode* pCNode = pNode->mChildren[i];
			InitNode(pCNode, pScene, relativeTransform);
		}
	}

	void ModelLoader::InitMesh(const aiMesh* pMesh, const aiMatrix4x4& transform)
	{
		// do not process mesh with POINTS or LINES primitives
		if(pMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
			return;

		const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<unsigned int> indices;

		vertices.reserve(pMesh->mNumVertices * 3);
		normals.reserve(pMesh->mNumVertices * 3);
		indices.reserve(pMesh->mNumVertices / 2);

		for(unsigned int i = 0; i < pMesh->mNumVertices; ++i)
		{
			const aiVector3D& pos = transform * pMesh->mVertices[i];	
			const aiVector3D& normal = pMesh->mNormals[i];	

			vertices.push_back(pos.x);
			vertices.push_back(pos.y);
			vertices.push_back(pos.z);

			normals.push_back(normal.x);
			normals.push_back(normal.y);
			normals.push_back(normal.z);
		}

		for(unsigned int i = 0; i < pMesh->mNumFaces; ++i)
		{
			const aiFace& face = pMesh->mFaces[i];

			for(unsigned int ii = 0; ii < 3; ++ii)
			{
				indices.push_back(face.mIndices[ii]);
			}
		}

		MeshT mesh(new Mesh);
		mesh->SetBufferData(ShaderLocation::Position, vertices, 3);
		mesh->SetBufferData(ShaderLocation::Normal, normals, 3);
		mesh->SetIndicesBufferData(indices);
		mesh->SetPrimitiveType(GL_TRIANGLES);

		Material material;
		material.SetAmbientColor( glm::vec3(0.2, 0.3, 0.9) );
		material.SetDiffuseColor( glm::vec3(0.2, 0.3, 0.9) );
		material.SetSpecularColor( glm::vec3(0.2, 0.3, 0.9) );
		material.SetSpecularShininess(10.0f);

		_actor->AddDrawableMesh(mesh, material);
	}
}
