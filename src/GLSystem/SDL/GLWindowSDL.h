#pragma once

#include "../GLWindow.h"
#include "../GLContext.h"
#include <SDL/SDL.h>

#include <functional>

class GLWindowSDL
{
public:
	GLWindowSDL(int argc, char** argv);
	~GLWindowSDL();

public:
	template<typename WindowT>
	void initWindow(WindowT& window, const unsigned int width, const unsigned int height);
	void run();

public:
	void SDLInitialize();
	void SDLResize(int width, int height);
	void SDLPaint();

	void ProcessEvents();
	void SDLMouseEvent(int buttons, int state, int x, int y);
	void SDLMouseMove(int x, int y);

private:
	bool _stop;
	unsigned int _initWidth;
	unsigned int _initHeight;

	std::function<void()> _init;
	std::function<void(int, int)> _resize;
	std::function<void()> _paint;

	std::function<void(int, int, int, int)> _mouseEvent;
	std::function<void(int, int)> _mouseMove;
};

#include "GLWindowSDL.hpp"
