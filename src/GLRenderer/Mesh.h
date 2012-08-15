#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <map>

namespace GLRenderer
{
	class Mesh
	{
		public:
			Mesh();
			~Mesh();

			Mesh(const Mesh& rhs) = delete;
			Mesh& operator=(const Mesh& rhs) = delete;

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
			};

			typedef std::map<int, VBODesc> VBOMap;
			VBOMap _buffers;
			VBODesc _indices;

			GLuint _vaoHandle;
			bool _vaoUptoDate;

			GLenum _primitiveT;
			unsigned int _verticesCount;

	};
}

#include "Mesh.hpp"
