#include "GLPrimitive3D.h"
#include "GLUtils.h"

#include <numeric>

GLPrimitive3D::GLPrimitive3D()
	: _vaoUptoDate(false), _verticesCount(0)
{
	// init vao
	glGenVertexArrays(1, &_vaoHandle);
}

GLPrimitive3D::~GLPrimitive3D()
{
	for(auto& vbo : _buffers)
		glDeleteBuffers(1, &vbo.second.handle);

	glDeleteVertexArrays(1, &_vaoHandle);
}

void GLPrimitive3D::SetPrimitiveType(const GLenum type)
{
	_primitiveT = type;
}

void GLPrimitive3D::BindTexture(const GLTextureT& texture)
{
	_textures.push_back(texture);
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

	// if no indices buffer, generate one
	if(_indices.size == 0)
	{
		std::vector<unsigned int> genIndices(_verticesCount);
		std::iota(genIndices.begin(), genIndices.end(), 0);

		glGenBuffers(1, &_indices.handle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices.handle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, genIndices.size() * sizeof(unsigned int),
			genIndices.data(), GL_STATIC_DRAW);

		_indices.size = _verticesCount;
	}

	_vaoUptoDate = true;
	GLUtils::CheckErrorAndPrint("Init");
}

void GLPrimitive3D::Draw()
{
	if(!_vaoUptoDate)
		Init();

	glBindVertexArray(_vaoHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices.handle);
	glDrawElements(_primitiveT, _indices.size, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLUtils::CheckErrorAndPrint("GLPrimitive3D::Draw");
}
