#pragma once

// Base class for all custom window declaration
class GLWindowBase
{
public:
	GLWindowBase() {}
	virtual ~GLWindowBase() {}

	GLWindowBase(const GLWindowBase& rhs) = delete;
	GLWindowBase& operator=(const GLWindowBase& rhs) =delete;

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
