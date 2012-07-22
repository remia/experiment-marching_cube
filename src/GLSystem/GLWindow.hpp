#include <iostream>

//#include "GLContext.h"
//#include <GL/gl.h>

template <typename T, typename WindowSystem>
GLWindow<T, WindowSystem>::GLWindow(int argc, char** argv)
    : WindowSystem(argc, argv)
{

}

template <typename T, typename WindowSystem>
GLWindow<T, WindowSystem>::~GLWindow()
{

}

template <typename T, typename WindowSystem>
void GLWindow<T, WindowSystem>::initWindow(const unsigned int width, const unsigned int height)
{
	std::cout << "GLWindow::InitWindow" << std::endl;
	WindowSystem::initWindow(*this, width, height);
}

template <typename T, typename WindowSystem>
void GLWindow<T, WindowSystem>::run()
{
	std::cout << "GLWindow::Run" << std::endl;
	WindowSystem::run();
}
