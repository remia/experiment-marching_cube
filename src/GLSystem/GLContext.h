#pragma once

#include <GL/glew.h>
#include <string>

class GLContext
{
public:
	static bool LoadExtensions(); // Glew init
	static std::string Info(); // Vendors, versions, ...
};
