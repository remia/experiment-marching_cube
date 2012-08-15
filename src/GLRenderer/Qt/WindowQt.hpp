#pragma once

#include "../Window.h"
#include "QtGLWidget.h"

#include <functional>
#include <memory>

#include <QApplication>

namespace GLRenderer
{
	class WindowQt
	{
		public:
			WindowQt(int argc, char** argv) : _qtApps(argc, argv) {}

		protected:
			template<typename WindowT>
				void initWindow(WindowT& window, const unsigned int width, const unsigned int height);

			void run();

		private:
			QApplication _qtApps;
			QtGLWidget _qtGLWidget;
	};

	template<typename WindowT>
		void WindowQt::initWindow(WindowT& window, const unsigned int width, const unsigned int height)
		{
			// Init window

			// Wire Qt class
			_qtGLWidget._init = std::bind(&WindowT::initialize, std::ref(window));
			_qtGLWidget._resize = std::bind(&WindowT::resize, std::ref(window));
			_qtGLWidget._paint = std::bind(&WindowT::paint, std::ref(window));

			_qtGLWidget.show();

		}

	void WindowQt::run()
	{
		// Launch Qt main loop
		_qtApps.exec();
	}
}
