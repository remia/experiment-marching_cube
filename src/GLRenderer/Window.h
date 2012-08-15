#pragma once

#include <type_traits>

namespace GLRenderer
{
	class WindowBase;

	// Host class
	// note : inheritance order matters, WindowSystem must get a chance
	// to initialize OpenGL context before T constructor get called, because
	// it might call OpenGL function during construction
	template <typename T,typename WindowSystem>
		class Window : public WindowSystem, public T
	{
		static_assert(std::is_base_of<WindowBase,T>::value, 
				"Window : 1st template argument must be derived from base class WindowBase");

		public:
		Window(int argc, char** argv);
		~Window();

		private:
		Window(const Window& rhs);
		Window& operator=(const Window& rhs);

		public:
		void initWindow(const unsigned int width, const unsigned int height);
		void run(); // main event loop
	};
}

#include "Window.hpp"
