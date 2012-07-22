#pragma once

template <typename T,typename WindowSystem>
class GLWindow : public T, public WindowSystem
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
