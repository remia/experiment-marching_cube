#include "Utils.h"

#include <iostream>
#include <type_traits>

namespace GLRenderer
{
	template <typename T>
		void Mesh::SetBufferData(const int location, const T* data, const unsigned int size,
				const unsigned int componentSize, const unsigned int stride, bool normalized)
		{
			// check vertices count
			if(_verticesCount == 0)
			{
				_verticesCount = size / componentSize;
			}
			else if(_verticesCount != (size / componentSize))
			{
				std::cerr << "Incorrect buffer size relative to prior buffers" << std::endl;
				return;
			}

			// retrieve / init corresponding VBO
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

			Utils::CheckErrorAndPrint("Mesh::SetBufferData");

			desc.location = location;
			desc.size = size;
			desc.componentSize = componentSize;
			desc.stride = stride;
			desc.normalized = normalized ? GL_TRUE : GL_FALSE;

			_buffers[location] = desc;
		}	

	template <typename T>
		void Mesh::SetBufferData(const int location, const std::vector<T>& vData,
				const unsigned int componentSize, const unsigned int stride, bool normalized)
		{
			SetBufferData(location, vData.data(), vData.size(), componentSize, stride, normalized);
		}

	template <typename T>
		void Mesh::SetIndicesBufferData(const T* data, const unsigned int size)
		{
			// current impl allow only indices of type unsigned int
			static_assert(std::is_same<T, unsigned int>::value, "SetIndicesBufferData : only 'unsigned int' allowed");	

			if(_indices.size == 0)
			{
				glGenBuffers(1, &_indices.handle);
			}

			// populate vbo
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indices.handle);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(T), data, GL_STATIC_DRAW);

			Utils::CheckErrorAndPrint("Mesh::SetIndicesBufferData");

			_indices.size = size;

		}

	template <typename T>
		void Mesh::SetIndicesBufferData(const std::vector<T>& vData)
		{
			SetIndicesBufferData(vData.data(), vData.size());
		}
}
