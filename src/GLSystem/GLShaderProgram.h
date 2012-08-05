#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>

class GLShaderProgram
{
public:
	GLShaderProgram();
	~GLShaderProgram();

	GLShaderProgram(const GLShaderProgram& rhs) = delete;
	GLShaderProgram& operator=(const GLShaderProgram& rhs) =delete;

public:
	bool AddShaderFromFile(const GLenum type, const std::string& path, std::string& log);

	void BindAttribLocation(const int location, const std::string& name);

	GLuint GetAttribLocation(const std::string& name);
	
	GLuint GetUniformLocation(const std::string& name);
	
	GLuint GetSubroutineIndex(const GLenum type, const std::string& name);

	bool Link(std::string& log);

	void Install();

	void PrintAttrib();

	void PrintUniform();

private:
	GLuint _programHandle;
	std::vector<GLuint> _shaderHandles;
};

