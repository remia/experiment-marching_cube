#include "WindowGlut.h"

namespace GLRenderer
{
	WindowGlut* GlutWrapper::_glutWindow = 0;

	WindowGlut::WindowGlut(int argc, char** argv)
	{
		GlutWrapper::_glutWindow = this;

		glutInit (&argc, argv);

		glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
		// glutCreateWindow seems to initialize opengl context,
		// call before GLContext::LoadExtensions
		glutCreateWindow("Glut window");

		Context::LoadExtensions();
		std::cout << Context::Info() << std::endl;
	}

	void WindowGlut::run()
	{
		_init();
		glutMainLoop();
	}

	void WindowGlut::glutInitialize()
	{
		_init();
	}

	void WindowGlut::glutResize(int width, int height)
	{
		_resize(width, height);
		glutPostRedisplay();
	}

	void WindowGlut::glutPaint()
	{
		_paint();
	}

	void WindowGlut::glutMouseEvent(int buttons, int state, int x, int y)
	{
		_mouseEvent(buttons, state, x, y);
	}

	void WindowGlut::glutMouseMove(int x, int y)
	{
		_mouseMove(x, y);
		glutPostRedisplay();
	}

	void WindowGlut::glutKeyboardEvent(unsigned char keys, int x, int y)
	{
		_keyboardEvent(keys, x, y);
		glutPostRedisplay();
	}
}
