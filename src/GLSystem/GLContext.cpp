#include "GLContext.h"

#include <iostream>
#include <sstream>

bool GLContext::LoadExtensions()
{
	// Init glew
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
	  	// problem : glewInit failed, something is seriously wrong
		std::cerr << "Error glew : " << glewGetErrorString(err) << std::endl;
		return false;
	}

	std::cout << "Status : Using GLEW : " << glewGetString(GLEW_VERSION) << std::endl;
	return true;
}

std::string GLContext::Info()
{
	// print opengl infos
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	std::ostringstream info;
	info 	<< "GL Vendor : " << vendor << std::endl
			<< "GL Renderer : " << renderer << std::endl
			<< "GL Version (str) : " << version << std::endl
			<< "GL Version (int) : " << major << "." << minor << std::endl
			<< "GLSL Version : " << glslVersion << std::endl;
	
	return info.str();
}
