#pragma once

#include "../Window.h"
#include "../Context.h"
#include <GL/glut.h>

#include <functional>

namespace GLRenderer
{
	class WindowGlut
	{
		public:
			WindowGlut(int argc, char** argv);

		public:
			template<typename WindowT>
				void initWindow(WindowT& window, const unsigned int width, const unsigned int height);
			void run();

		public:
			void glutInitialize();
			void glutResize(int width, int height);
			void glutPaint();

			void glutMouseEvent(int buttons, int state, int x, int y);
			void glutMouseMove(int x, int y);
			void glutKeyboardEvent(unsigned char keys, int x, int y);

		private:
			std::function<void()> _init;
			std::function<void(int, int)> _resize;
			std::function<void()> _paint;

			std::function<void(int, int, int, int)> _mouseEvent;
			std::function<void(int, int)> _mouseMove;
			std::function<void(unsigned char, int, int)> _keyboardEvent;
	};

	class GlutWrapper
	{
		public:
			static void resize(int width, int height) { _glutWindow->glutResize(width, height); }
			static void paint() { _glutWindow->glutPaint(); }

			static void mouseEvent(int buttons, int state, int x, int y) { _glutWindow->glutMouseEvent(buttons, state, x, y); }
			static void mouseMove(int x, int y) { _glutWindow->glutMouseMove(x, y); }
			static void keyboardEvent(unsigned char keys, int x, int y) { _glutWindow->glutKeyboardEvent(keys, x, y); }

			static WindowGlut* _glutWindow;
	};
}

#include "WindowGlut.hpp"
