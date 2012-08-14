#pragma once

#include "GLPrimitiveLibrary.h"

#include <string>
#include <vector>

// TODO : hide implementation details
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/matrix4x4.h>

class GLModel : public GLDrawable
{
public:
	explicit GLModel(const std::string& path);
	~GLModel();

	GLModel(const GLModel& rhs) = delete;
	GLModel& operator=(const GLModel& rhs) = delete;

public:
	void Draw();

private:
	void LoadFromFile(const std::string& path);
	void InitScene(const aiScene* pScene);
	void InitNode(const aiNode* pNode, const aiScene* pScene, const aiMatrix4x4& transform);
	void InitMesh(const aiMesh* pMesh, const aiMatrix4x4& transform);

private:
	typedef GLEntityT Mesh;
	std::vector<Mesh> _meshs;
};
