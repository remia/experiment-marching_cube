#include <iostream>

namespace GLRenderer
{
	template <typename T, typename WindowSystem>
		Window<T, WindowSystem>::Window(int argc, char** argv)
		: WindowSystem(argc, argv), T()
		{

		}

	template <typename T, typename WindowSystem>
		Window<T, WindowSystem>::~Window()
		{

		}

	template <typename T, typename WindowSystem>
		void Window<T, WindowSystem>::initWindow(const unsigned int width, const unsigned int height)
		{
			std::cout << "Window::InitWindow" << std::endl;
			WindowSystem::initWindow(*this, width, height);
		}

	template <typename T, typename WindowSystem>
		void Window<T, WindowSystem>::run()
		{
			std::cout << "Window::Run" << std::endl;
			WindowSystem::run();
		}
}
