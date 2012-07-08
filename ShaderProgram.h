#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <vector>
#include <string>

class ShaderProgram
{
public:
	explicit ShaderProgram();
	~ShaderProgram();

private:
	explicit ShaderProgram(const ShaderProgram& rhs);
	ShaderProgram& operator=(const ShaderProgram& rhs);

public:
	bool AddShaderFromFile(const GLenum type, const std::string& path, std::string& log);

	void BindAttribLocation(const int location, const std::string& name);

	GLuint GetUniformLocation(const std::string& name);
	
	bool Link(std::string& log);

	void Install();

	void PrintAttrib();

	void PrintUniform();

private:
	GLuint _programHandle;
	std::vector<GLuint> _shaderHandles;
};

