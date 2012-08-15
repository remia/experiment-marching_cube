#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

namespace GLRenderer
{
	class Texture
	{
		public:
			Texture();
			~Texture();

			Texture(const Texture& rhs) = delete;
			Texture& operator=(const Texture& rhs) = delete;

		public:
			bool LoadFromFile(const std::string& filename);

		private:
			GLuint _handle;
			GLenum _target;
	};
}
