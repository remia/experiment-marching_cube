#pragma once

// Host class
// note : inheritance order matters, WindowSystem must get a chance
// to initialize OpenGL context before T constructor get called, because
// it might call OpenGL function during construction
template <typename T,typename WindowSystem>
class GLWindow : public WindowSystem, public T
{
public:
    	GLWindow(int argc, char** argv);
	~GLWindow();

private:
	GLWindow(const GLWindow& rhs);
	GLWindow& operator=(const GLWindow& rhs);

public:
	void initWindow(const unsigned int width, const unsigned int height);
	void run(); // main event loop
};

#include "GLWindow.hpp"
