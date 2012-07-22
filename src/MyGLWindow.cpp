//#include <GL/glew.h>

#include "MyGLWindow.h"
#include "MarchingCube.h"

#include <iostream>
#include <chrono>

MyGLWindow::MyGLWindow()
	: xRotate(0), yRotate(0), zRotate(0)
{

}

MyGLWindow::~MyGLWindow()
{

}

void MyGLWindow::initialize()
{
	// load shaders
	std::string vertexLog;
	bool vres = sprog.AddShaderFromFile(GL_VERTEX_SHADER,"./src/vertex.glsl", vertexLog);
	if( !vres ) std::cout << vertexLog << std::endl;

	std::string fragmentLog;
	bool fres = sprog.AddShaderFromFile(GL_FRAGMENT_SHADER, "./src/fragment.glsl", fragmentLog);
	if( !fres ) std::cout << fragmentLog << std::endl;

	const int PositionLocation = 0;
	const int ColorLocation = 1;
	const int NormalLocation = 2;
	sprog.BindAttribLocation(PositionLocation, "VertexPosition");
	sprog.BindAttribLocation(ColorLocation, "VertexColor");
	sprog.BindAttribLocation(NormalLocation, "VertexNormal");

	std::string linkLog;
	bool lres = sprog.Link(linkLog);
	if( !lres ) std::cout << linkLog << std::endl;

	// install the program in OpenGL pipeline
	sprog.Install();
	
	// print actives attributes
	sprog.PrintAttrib();
	sprog.PrintUniform();

	// timings
	std::chrono::time_point<std::chrono::system_clock> start, end;
	int ellapsed_milliseconds;

	// initialize marching cube
	start = std::chrono::system_clock::now();
	
	float minValue = 0; //1.8f;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	minX = minY = minZ = -8.0f;
	maxX = maxY = maxZ = 8.0f;

	unsigned int nX, nY, nZ;
	nX = nY = nZ = 100;

	std::vector<glm::vec4> mcPoints((nX + 1) * (nY + 1) * (nZ + 1));
	glm::vec3 mcStep((maxX - minX) / nX, (maxY - minY) / nY, (maxZ - minZ) / nZ);

	for(int x = 0; x < nX + 1; ++x)
		for(int y = 0; y < nY + 1; ++y)
			for(int z = 0; z < nZ + 1; ++z)
			{
				glm::vec4 point(
					minX + x * mcStep.x,
					minY + y * mcStep.y,
					minZ + z * mcStep.z, 0);
			
				point.w = Potential(point.x, point.y, point.z);
				mcPoints[x * (nY + 1) * (nZ + 1) + y * (nZ + 1) + z] = point;
			}

	end = std::chrono::system_clock::now();
	ellapsed_milliseconds = 
		std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
	std::cout << "Initialization timer (ms) : " << ellapsed_milliseconds << std::endl;

	// run marching cube
	start = std::chrono::system_clock::now();

	std::vector<Triangle> mcTriangles;

	MarchingCube mc;
	mc.SetMinValue(minValue);
	mc.SetDimension(nX, nY, nZ);
	mc.Process(mcPoints, mcTriangles);
	
	end = std::chrono::system_clock::now();
	ellapsed_milliseconds = 
		std::chrono::duration_cast<std::chrono::milliseconds>
			(end-start).count();
	std::cout << "Marching cube timer (ms) : " << ellapsed_milliseconds << std::endl;

	triangleCount = mcTriangles.size();
	std::cout << "Point count : " << mcPoints.size() << std::endl;
	std::cout << "Triangle count : " << triangleCount << std::endl;

	// initialize data
	std::vector<float> positionArray;
	std::vector<float> colorArray;
	std::vector<float> normalArray;

	glm::vec3 color(1.0f, 1.0f, 0.0f);
	for(auto t : mcTriangles)
	{
		for(int i = 0; i < 3; ++i)
		{
			positionArray.push_back(t.vertices[i].x);
			positionArray.push_back(t.vertices[i].y);
			positionArray.push_back(t.vertices[i].z);

			colorArray.push_back(color.x);
			colorArray.push_back(color.y);
			colorArray.push_back(color.z);

			normalArray.push_back(t.normal[i].x);
			normalArray.push_back(t.normal[i].y);
			normalArray.push_back(t.normal[i].z);
		}
	}

	// debug, display 3 axis
/*
	positionArray = std::vector<float> {
		 0.0f,  0.0f, 0.0f,
		 2.0f,  0.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.0f,  2.0f, 0.0f,
		 0.0f,  0.0f, 0.0f,
		 0.0f,  0.0f, 2.0f };

	colorArray = std::vector<float> {
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f };
*/

	// init vbo
	GLuint vboHandles[3];
	glGenBuffers(3, vboHandles);
	GLuint positionBufferHandle = vboHandles[0];
	GLuint colorBufferHandle = vboHandles[1];
	GLuint normalBufferHandle = vboHandles[2];

	// populate vbo
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(float),
		 positionArray.data(), GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	//glBufferData(GL_ARRAY_BUFFER, colorArray.size() * sizeof(float),
	//	 colorArray.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(float),
		 normalArray.data(), GL_STATIC_DRAW);

	// init vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);	

	glEnableVertexAttribArray(PositionLocation);
	//glEnableVertexAttribArray(ColorLocation);
	glEnableVertexAttribArray(NormalLocation);

	// map corresponding vbo to vao attribArray
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(PositionLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	//glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	//glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "Initialize : OpenGL Error: %s\n", errString);
	}
}

void MyGLWindow::resize(int width, int height)
{
	if (height == 0) height = 1;
	glViewport (0, 0, (GLsizei) width, (GLsizei) height);

	projectionMatrix = glm::perspective(45.0f, (1.0f * width) / (1.0f * height), 0.1f, 100.0f);
}

void MyGLWindow::paint()
{
	glEnable(GL_DEPTH_TEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	// set matrices model view proj
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
	modelMatrix = glm::rotate(modelMatrix, xRotate / 16.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, yRotate / 16.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrix = glm::rotate(modelMatrix, zRotate / 16.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 2.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -10.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;	
	glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
	glm::mat4 pmvMatrix = projectionMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(sprog.GetUniformLocation("ProjectionMatrix"),
		1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix3fv(sprog.GetUniformLocation("NormalMatrix"),
		1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(sprog.GetUniformLocation("ModelViewMatrix"),
		1, GL_FALSE, glm::value_ptr(modelViewMatrix));
	glUniformMatrix4fv(sprog.GetUniformLocation("MVP"),
		1, GL_FALSE, glm::value_ptr(pmvMatrix));

	// set shader lighting params
	struct LightInfo
	{
		glm::vec4 Position;	// Light position in eye coords
		glm::vec3 La;		// Ambient light intensity
		glm::vec3 Ld;		// Diffuse light intensity
		glm::vec3 Ls;		// Specular light intensity;
	};
	LightInfo Light;
	Light.Position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
	Light.La = glm::vec3(0.1f, 0.1f, 0.7f);
	Light.Ld = glm::vec3(0.8f, 0.5f, 0.5f);
	Light.Ls = glm::vec3(1.0f, 1.0f, 1.0f);

	struct MaterialInfo
	{
		glm::vec3 Ka;			// Ambient reflectivity
		glm::vec3 Kd;			// Diffuse reflectivity
		glm::vec3 Ks;			// Specular reflectivity
		float Shininess;	// Specular shininess factor
	};
	MaterialInfo Material;
	Material.Ka = glm::vec3(0.1f, 0.1f, 0.7f);
	Material.Kd = glm::vec3(0.2f, 0.2f, 0.2f);
	Material.Ks = glm::vec3(1.0f, 1.0f, 1.0f);
	Material.Shininess = 30.0f;

	glUniform4fv(sprog.GetUniformLocation("Light.Position"), 1, glm::value_ptr(Light.Position));
	glUniform3fv(sprog.GetUniformLocation("Light.La"), 1, glm::value_ptr(Light.La));
	glUniform3fv(sprog.GetUniformLocation("Light.Ld"), 1, glm::value_ptr(Light.Ld));
	glUniform3fv(sprog.GetUniformLocation("Light.Ls"), 1, glm::value_ptr(Light.Ls));
	
	glUniform3fv(sprog.GetUniformLocation("Material.Ka"), 1, glm::value_ptr(Material.Ka));
	glUniform3fv(sprog.GetUniformLocation("Material.Kd"), 1, glm::value_ptr(Material.Kd));
	glUniform3fv(sprog.GetUniformLocation("Material.Ks"), 1, glm::value_ptr(Material.Ks));
	glUniform1f(sprog.GetUniformLocation("Material.Shininess"), Material.Shininess);

	GLuint diffuseLocation = sprog.GetSubroutineIndex(GL_VERTEX_SHADER, "diffuseOnly"); // "phongShading");
	glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseLocation);

	// draw
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
	//glDrawArrays(GL_LINES, 0, 3 * 3);
	glFlush();

	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "Draw OpenGL Error: %s\n", errString);
	}
}

// no input abstraction implemented
#include <GL/glut.h>
void MyGLWindow::mouseEvent(int buttons, int state, int x, int y)
{
	if( buttons == GLUT_LEFT_BUTTON)
	{
		lastMousePos.x = x;
		lastMousePos.y = y;
	}
}

void MyGLWindow::mouseMove(int x, int y)
{
	int dx = x - lastMousePos.x;
	int dy = y - lastMousePos.y;

	xRotate += dy * 8;
	yRotate += dx * 8;

	lastMousePos.x = x;
	lastMousePos.y = y;
}

// used by marching cube initialization
float MyGLWindow::Potential(const float x, const float y, const float z)
{
	float a = 0;
	float b = -2;
	float c = 1;

	return powf(x,4) + powf(y,4) + powf(z,4)
		+ a * ( x*x + y*y + z*z )
		+ b * ( x*x + y*y + z*z )
		+ c;
	
	//return sqrtf( x*x + y*y + z*z );
}
