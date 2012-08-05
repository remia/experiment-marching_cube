#include "GLTexture.h"

#include <FreeImagePlus.h>

#include <iostream>

GLTexture::GLTexture()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_handle);
}

GLTexture::~GLTexture()
{
	glDeleteTextures(1, &_handle);
}

bool GLTexture::LoadFromFile(const std::string& filename)
{
	fipImage tImg;
	bool success = tImg.load(filename.c_str());

	if(!success)
	{
		std::cerr << "Unable to open file : " << filename << std::endl;
		return false;
	}

	// TODO :check image dimension <=> power of two?
	glBindTexture(GL_TEXTURE_2D, _handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tImg.getWidth(), tImg.getHeight(),
		0, GL_RGB, GL_UNSIGNED_BYTE, tImg.accessPixels());

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}
