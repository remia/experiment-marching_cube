#include "GLRenderer/Glut/WindowGlut.h"
//#include "GLRenderer/SDL/WindowSDL.h"
//#include "GLRenderer/Qt/WindowQt.hpp"

#include "MyGLWindow.h"

int main(int argc, char** argv)
{
	GLRenderer::Window<MyGLWindow, GLRenderer::WindowGlut> win(argc, argv);
	win.initWindow(500, 500);
	win.run();

	return 0;
}
