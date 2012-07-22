#pragma once

#include "../GLWindow.h"
#include "QtGLWidget.h"

#include <functional>
#include <memory>

#include <QApplication>

class GLWindowQt
{
public:
    GLWindowQt(int argc, char** argv) : _qtApps(argc, argv) {}

protected:
    template<typename WindowT>
    void initWindow(WindowT& window, const unsigned int width, const unsigned int height);

    void run();

private:
    QApplication _qtApps;
    QtGLWidget _qtGLWidget;
};

template<typename WindowT>
void GLWindowQt::initWindow(WindowT& window, const unsigned int width, const unsigned int height)
{
    // Init window

	// Wire Qt class
    _qtGLWidget._init = std::bind(&WindowT::initialize, std::ref(window));
    _qtGLWidget._resize = std::bind(&WindowT::resize, std::ref(window));
    _qtGLWidget._paint = std::bind(&WindowT::paint, std::ref(window));

    _qtGLWidget.show();

}

void GLWindowQt::run()
{
	// Launch Qt main loop
    _qtApps.exec();
}
