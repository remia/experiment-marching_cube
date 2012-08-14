#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include "GLDrawable.h"
#include "GLTextureLibrary.h"

#include <vector>
#include <map>
#include <string>

class GLPrimitive3D : public GLDrawable
{
public:
	GLPrimitive3D();
	~GLPrimitive3D();

	GLPrimitive3D(const GLPrimitive3D& rhs) = delete;
	GLPrimitive3D& operator=(const GLPrimitive3D& rhs) = delete;

public:
	template <typename T>
	void SetBufferData(const int location, const T* data, const unsigned int size,
		const unsigned int componentSize, const unsigned int stride = 0, bool normalized = false);

	template <typename T>
	void SetBufferData(const int location, const std::vector<T>& vData,
		const unsigned int componentSize, const unsigned int stride = 0, bool normalized = false);

	template <typename T>
	void SetIndicesBufferData(const T* data, const unsigned int size);

	template <typename T>
	void SetIndicesBufferData(const std::vector<T>& vData);

	void BindTexture(const GLTextureT& texture);

	void SetPrimitiveType(const GLenum type);

public:
	void Init();
	virtual void Draw();

private:
	struct VBODesc
	{
		VBODesc() : location(0), size(0) {}

		int location;
		unsigned int size;
		unsigned int componentSize;
		unsigned int stride;
		unsigned int offset;
		bool normalized;
		
		GLenum type;
		GLuint handle;
		std::string description;
	};

	typedef std::map<int, VBODesc> VBOMap;
	typedef std::vector<GLTextureT> TextureList;

	VBOMap _buffers;
	VBODesc _indices;

	TextureList _textures;

	GLuint _vaoHandle;
	bool _vaoUptoDate;

	GLenum _primitiveT;
	unsigned int _verticesCount;

};

#include "GLPrimitive3D.hpp"
