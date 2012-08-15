#include "WindowSDL.h"

namespace GLRenderer
{
	WindowSDL::WindowSDL(int argc, char** argv)
		: _stop(false), _initWidth(100), _initHeight(100)
	{
		putenv(const_cast<char*>("SDL_VIDEO_CENTERED=1"));

		SDL_Init(SDL_INIT_VIDEO);
		SDL_SetVideoMode(_initWidth, _initHeight, 32,
				SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE);

		Context::LoadExtensions();
		std::cout << Context::Info() << std::endl;
	}

	WindowSDL::~WindowSDL()
	{
		SDL_Quit();
	}

	void WindowSDL::run()
	{
		SDLInitialize();
		SDLResize(_initWidth, _initHeight);

		// main loop
		while(!_stop)
		{
			ProcessEvents();
			SDLPaint();
		}
	}

	void WindowSDL::SDLInitialize()
	{
		_init();
	}

	void WindowSDL::SDLResize(int width, int height)
	{
		SDL_SetVideoMode(width, height, 32,
				SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE);
		_resize(width, height);
	}

	void WindowSDL::SDLPaint()
	{
		_paint();
		SDL_GL_SwapBuffers();
	}

	void WindowSDL::ProcessEvents()
	{
		SDL_Event event;

		while( SDL_PollEvent(&event) )
		{
			switch(event.type)
			{
				case SDL_VIDEORESIZE :
					{
						std::cout << "Resize!" << std::endl;
						//SDLResize(event.resize.w, event.resize.h);
					}
					break;

				case SDL_KEYDOWN :
					break;

				case SDL_QUIT :
					_stop = true;
					break;
			}
		}
	}

	void WindowSDL::SDLMouseEvent(int buttons, int state, int x, int y)
	{
		_mouseEvent(buttons, state, x, y);
	}

	void WindowSDL::SDLMouseMove(int x, int y)
	{
		_mouseMove(x, y);
	}
}
