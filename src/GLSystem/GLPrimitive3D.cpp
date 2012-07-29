#include "GLPrimitive3D.h"

#include "GLUtils.h"

GLPrimitive3D::GLPrimitive3D()
	: _vaoUptoDate(false), _verticesCount(0)
{
	// init vao
	glGenVertexArrays(1, &_vaoHandle);
}

GLPrimitive3D::~GLPrimitive3D()
{
	// TODO : destroy GLBuffers / Arrays
}

void GLPrimitive3D::SetPrimitiveType(const GLenum type)
{
	_primitiveT = type;
}

void GLPrimitive3D::SetVerticesCount(const unsigned int count)
{
	_verticesCount = count;
}

void GLPrimitive3D::Init()
{
	glBindVertexArray(_vaoHandle);

	for(auto bufferEntry : _buffers)
	{
		const VBODesc& desc = bufferEntry.second;
		glEnableVertexAttribArray(desc.location);

		glBindBuffer(GL_ARRAY_BUFFER, desc.handle);
		glVertexAttribPointer(desc.location, desc.componentSize, GL_FLOAT,
			desc.normalized, desc.stride, (GLubyte*) NULL);
	}

	_vaoUptoDate = true;
	GLUtils::CheckErrorAndPrint();
}

void GLPrimitive3D::Draw()
{
	if(!_vaoUptoDate)
		Init();

	glBindVertexArray(_vaoHandle);
	glDrawArrays(_primitiveT, 0, _verticesCount);

	GLUtils::CheckErrorAndPrint();
}
