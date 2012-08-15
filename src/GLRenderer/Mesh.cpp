#include "Mesh.h"
#include "Utils.h"

#include <numeric>

namespace GLRenderer
{
	Mesh::Mesh()
		: _vaoUptoDate(false), _verticesCount(0)
	{
		// init vao
		glGenVertexArrays(1, &_vaoHandle);
	}

	Mesh::~Mesh()
	{
		for(auto& vbo : _buffers)
			glDeleteBuffers(1, &vbo.second.handle);

		if(_indices.size != 0)
			glDeleteBuffers(1, &_indices.handle);

		glDeleteVertexArrays(1, &_vaoHandle);
	}

	void Mesh::SetPrimitiveType(const GLenum type)
	{
		_primitiveT = type;
	}

	void Mesh::Init()
	{
		// TODO : VBO type = GL_FLOAT hardcoded
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
		Utils::CheckErrorAndPrint("Init");
	}

	void Mesh::Draw()
	{
		if(!_vaoUptoDate)
			Init();

		glBindVertexArray(_vaoHandle);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices.handle);
		glDrawElements(_primitiveT, _indices.size, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		Utils::CheckErrorAndPrint("Mesh::Draw");
	}
}
