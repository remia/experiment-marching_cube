
template<typename WindowT>
void GLWindowGlut::initWindow(WindowT& window, const unsigned int width, const unsigned int height)
{
	// Init window
	glutReshapeWindow(width, height);

	// Bind window function
	using namespace std::placeholders;

	_init = std::bind(&WindowT::initialize, std::ref(window));
	_resize = std::bind(&WindowT::resize, std::ref(window), _1, _2);
	_paint = std::bind(&WindowT::paint, std::ref(window));

	_mouseEvent = std::bind(&WindowT::mouseEvent, std::ref(window), _1, _2, _3, _4);
	_mouseMove = std::bind(&WindowT::mouseMove, std::ref(window), _1, _2);
	
	// Bind GLWindow function to GLUT events
	glutReshapeFunc(GlutWrapper::resize);
	glutDisplayFunc(GlutWrapper::paint);

	glutMouseFunc(GlutWrapper::mouseEvent);
	glutMotionFunc(GlutWrapper::mouseMove);
}

