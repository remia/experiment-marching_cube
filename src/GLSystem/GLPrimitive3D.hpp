#include "GLUtils.h"

#include <iostream>
template <typename T>
void GLPrimitive3D::SetBufferData(const int location, const T* data, const unsigned int size,
	const unsigned int componentSize, const unsigned int stride, bool normalized)

{
	VBODesc desc;

	auto it = _buffers.find(location);	
	if( it == _buffers.end() )
	{
		glGenBuffers(1, &desc.handle);
		_vaoUptoDate = false;
	}
	else
	{
		desc = it->second;
	}

	// populate vbo
	glBindBuffer(GL_ARRAY_BUFFER, desc.handle);
	glBufferData(GL_ARRAY_BUFFER, size * sizeof(T), data, GL_STATIC_DRAW);

	GLUtils::CheckErrorAndPrint();

	desc.location = location;
	desc.componentSize = componentSize;
	desc.stride = stride;
	desc.normalized = normalized ? GL_TRUE : GL_FALSE;
	desc.description = "";

	_buffers[location] = desc;
}	

template <typename T>
void GLPrimitive3D::SetBufferData(const int location, const std::vector<T>& vData,
	const unsigned int componentSize, const unsigned int stride, bool normalized)
{
	SetBufferData(location, vData.data(), vData.size(), componentSize, stride, normalized);
}

