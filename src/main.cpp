#include "GLSystem/Glut/GLWindowGlut.h"
//#include "GLSystem/Qt/GLWindowQt.hpp"

#include "MyGLWindow.h"

int main(int argc, char** argv)
{
	GLWindow<MyGLWindow, GLWindowGlut> win(argc, argv);
	win.initWindow(500, 500);
	win.run();

	return 0;
}
