#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>

namespace GLRenderer
{
	class ShaderProgram
	{
		public:
			ShaderProgram();
			~ShaderProgram();

			ShaderProgram(const ShaderProgram& rhs) = delete;
			ShaderProgram& operator=(const ShaderProgram& rhs) = delete;

		public:
			bool AddShaderFromFile(const GLenum type, const std::string& path, std::string& log);

			void BindAttribLocation(const int location, const std::string& name);

			GLuint GetAttribLocation(const std::string& name) const;

			GLuint GetUniformLocation(const std::string& name) const;

			GLuint GetUniformBlockIndex(const std::string& name) const;

			GLint GetUniformBlockSize(const std::string& name) const;

			GLint GetUniformBlockOffset(const std::string& name, const std::string& variable) const;

			void BindUniformBlock(const std::string& name, const GLuint bindingIndex);

			GLuint GetSubroutineIndex(const GLenum type, const std::string& name) const;

			bool Link(std::string& log);

			void Install() const;

			void Release() const;

			void PrintAttrib() const;

			void PrintUniform() const;

		private:
			GLuint _programHandle;
			std::vector<GLuint> _shaderHandles;
	};
}
