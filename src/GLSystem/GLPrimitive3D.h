#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <map>
#include <string>

class GLPrimitive3D
{
public:
	GLPrimitive3D();
	~GLPrimitive3D();

	static GLPrimitive3D FromFile();

public:
	template <typename T>
	void SetBufferData(const int location, const T* data, const unsigned int size,
		const unsigned int componentSize, const unsigned int stride = 0, bool normalized = false);

	template <typename T>
	void SetBufferData(const int location, const std::vector<T>& vData,
		const unsigned int componentSize, const unsigned int stride = 0, bool normalized = false);

	void SetPrimitiveType(const GLenum type);

	void SetVerticesCount(const unsigned int count);

public:
	void Init();
	void Draw();

private:	
	struct VBODesc
	{
		int location;
		unsigned int componentSize;
		unsigned int stride;
		bool normalized;

		GLuint handle;
		std::string description;
	};

	typedef std::map<int, VBODesc> VBOMap; 
	VBOMap _buffers;

	GLuint _vaoHandle;
	bool _vaoUptoDate;

	GLenum _primitiveT;
	unsigned int _verticesCount;

};

#include "GLPrimitive3D.hpp"
