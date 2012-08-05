#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>

class GLTexture
{
public:
	GLTexture();
	~GLTexture();

public:
	bool LoadFromFile(const std::string& filename);

private:
	GLuint _handle;
	GLenum _target;
	std::string _description;
};
