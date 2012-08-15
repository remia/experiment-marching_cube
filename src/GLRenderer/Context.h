#pragma once

#include <GL/glew.h>
#include <string>

namespace GLRenderer
{
	class Context
	{
		public:
			static bool LoadExtensions(); // Glew init
			static std::string Info(); // Vendors, versions, ...
	};
}
