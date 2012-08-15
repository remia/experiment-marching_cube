#pragma once

namespace GLRenderer
{
	// Base class for all custom window declaration
	class WindowBase
	{
		public:
			WindowBase() {}
			virtual ~WindowBase() {}

			WindowBase(const WindowBase& rhs) = delete;
			WindowBase& operator=(const WindowBase& rhs) = delete;

		public:
			// GL default events
			virtual void initialize() = 0;
			virtual void resize(int width, int height) = 0;
			virtual void paint() = 0;

			// Optional Input event
			virtual void mouseEvent(int buttons, int state, int x, int y) {}
			virtual void mouseMove(int x, int y) {}
			virtual void keyboardEvent(unsigned char keys, int x, int y) {}
	};
}
