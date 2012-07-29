#include "GLUtils.h"

#include <GL/glew.h>
#include <GL/gl.h>

#include <iostream>

void GLUtils::CheckErrorAndPrint()
{
	GLenum errCode;
	const GLubyte *errString;

	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		std::cerr << "OpenGL Error : " << errString << std::endl;
	}
}
