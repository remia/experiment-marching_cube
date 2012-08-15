#pragma once

#include <QtOpenGL/QGLWidget>

#include <functional>
#include <memory>

class QtGLWidget : public QGLWidget
{
    Q_OBJECT

public:
    explicit QtGLWidget(QWidget* parent = 0) : QGLWidget(parent) {}
    ~QtGLWidget() {}

    QSize sizeHint() const { return QSize(200,200); }

protected:
    void initializeGL() {qglClearColor(Qt::darkBlue); _init(); }
    void paintGL() { glClear(GL_COLOR_BUFFER_BIT);  _paint(); }
    void resizeGL(int width, int height) { glViewport(0, 0, width, height); _resize(); }

public:
    std::function<void()> _init;
    std::function<void()> _paint;
    std::function<void()> _resize;
};
