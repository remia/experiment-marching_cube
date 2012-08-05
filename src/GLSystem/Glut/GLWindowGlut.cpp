#include "GLWindowGlut.h"

GLWindowGlut* GlutWrapper::_glutWindow = 0;

GLWindowGlut::GLWindowGlut(int argc, char** argv)
{
	GlutWrapper::_glutWindow = this;

	glutInit (&argc, argv);

	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	// glutCreateWindow seems to initialize opengl context,
	// call before GLContext::LoadExtensions
	glutCreateWindow("Glut window");
	
	GLContext::LoadExtensions();
	std::cout << GLContext::Info() << std::endl;
}

void GLWindowGlut::run()
{
	_init();
	glutMainLoop();
}

void GLWindowGlut::glutInitialize()
{
	_init();
}

void GLWindowGlut::glutResize(int width, int height)
{
	_resize(width, height);
	glutPostRedisplay();
}

void GLWindowGlut::glutPaint()
{
	_paint();
}
	
void GLWindowGlut::glutMouseEvent(int buttons, int state, int x, int y)
{
	_mouseEvent(buttons, state, x, y);
}

void GLWindowGlut::glutMouseMove(int x, int y)
{
	_mouseMove(x, y);
	glutPostRedisplay();
}

void GLWindowGlut::glutKeyboardEvent(unsigned char keys, int x, int y)
{
	_keyboardEvent(keys, x, y);
	glutPostRedisplay();
}
