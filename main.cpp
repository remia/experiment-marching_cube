#include <GL/glew.h>
#include <GL/glut.h>

// enable vector.x, .xyz, ... accessing style
#define GLM_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderProgram.h"
#include "MarchingCube.h"

#include <iostream>
 
GLuint vaoHandle;
ShaderProgram* sprog;
int triangleCount;

glm::mat4 projectionMatrix;

// used by marching cube initialization
float Potential(const float x, const float y, const float z)
{
/*
	glm::vec3 p(x, y, z);
	glm::vec3 dp1 = glm::vec3( 0.0, -2.0,  0.0) - p;
	glm::vec3 dp2 = glm::vec3( 0.0,  2.0,  0.0) - p;
	glm::vec3 dp3 = glm::vec3( 2.0,  2.0,  0.0) - p;
	glm::vec3 dp4 = glm::vec3( 0.0,  0.0,  4.0) - p;
	glm::vec3 dp5 = glm::vec3(-0.5,  3.1, -1.0) - p;
	glm::vec3 dp6 = glm::vec3( 0.0,  0.0, -4.0) - p;

	return 
		(1 / glm::length(dp1) + 1 / glm::length(dp2) + 1 / glm::length(dp3) 
		+ 1 / glm::length(dp4) + 1 / glm::length(dp5) + 1 / glm::length(dp6) );
*/
	return sqrtf( x*x + y*y + z*z );
}

void reshape (int w, int h)
{
	if (h == 0) h = 1;
	glViewport (0, 0, (GLsizei)w, (GLsizei)h);

	float ratio = (1.0f * h) / (1.0f *  w);
	projectionMatrix = glm::perspective(45.0f, (1.0f * w) / (1.0f * h), 0.1f, 100.0f);

	glutPostRedisplay();
}

void display()
{
	glEnable(GL_DEPTH_TEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	// set matrices model view proj
	glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 viewMatrix = glm::lookAt(
		glm::vec3(0.0f, 2.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -5.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	
	glm::mat4 pmvMatrix = projectionMatrix * viewMatrix * modelMatrix;

	glUniformMatrix4fv(sprog->GetUniformLocation("ProjModelViewMatrix"),
		1, GL_FALSE, glm::value_ptr(pmvMatrix));

	// set shader lighting params
	glm::vec3 lightDir(0.2f, 0.5f, 0.0f);
	glUniform3fv(sprog->GetUniformLocation("LightDirection"), 1, glm::value_ptr(lightDir));

	// draw
	glBindVertexArray(vaoHandle);
	glDrawArrays(GL_TRIANGLES, 0, triangleCount * 3);
	//glDrawArrays(GL_LINES, 0, 3 * 3);

	glFlush();

	GLenum errCode;
	const GLubyte *errString;
	if ((errCode = glGetError()) != GL_NO_ERROR) {
		errString = gluErrorString(errCode);
	   fprintf (stderr, "OpenGL Error: %s\n", errString);
	}
}

int main (int argc, char *argv[])
{
	// init GLUT
	glutInit (&argc, argv);

	glutInitDisplayMode (GLUT_RGBA | GLUT_SINGLE | GLUT_DEPTH);
	glutInitWindowSize (500, 500);
	glutCreateWindow ("Marching Cube");

	// print opengl infos
	const GLubyte *renderer = glGetString(GL_RENDERER);
	const GLubyte *vendor = glGetString(GL_VENDOR);
	const GLubyte *version = glGetString(GL_VERSION);
	const GLubyte *glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	printf("GL Vendor :\t%s\n", vendor);
	printf("GL Renderer :\t%s\n", renderer);
	printf("GL Version (string) :\t%s\n", version);
	printf("GL Version (integer) :\t%d.%d\n", major, minor);
	printf("GLSL Version :\t%s\n", glslVersion);

	// init GLEW
	GLenum err = glewInit();
	if(GLEW_OK != err)
	{
	  // problem: glewInit failed, something is seriously wrong
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status : Using GLEW %s\n", glewGetString(GLEW_VERSION));

	// load shaders
	sprog = new ShaderProgram();

	std::string vertexLog;
	bool vres = sprog->AddShaderFromFile(GL_VERTEX_SHADER,"vertex.glsl", vertexLog);
	if( !vres ) std::cout << vertexLog << std::endl;

	std::string fragmentLog;
	bool fres = sprog->AddShaderFromFile(GL_FRAGMENT_SHADER, "fragment.glsl", fragmentLog);
	if( !fres ) std::cout << fragmentLog << std::endl;

	const int PositionLocation = 0;
	const int ColorLocation = 1;
	const int NormalLocation = 2;
	sprog->BindAttribLocation(PositionLocation, "VertexPosition");
	sprog->BindAttribLocation(ColorLocation, "VertexColor");
	sprog->BindAttribLocation(NormalLocation, "VertexNormal");

	std::string linkLog;
	bool lres = sprog->Link(linkLog);
	if( !lres ) std::cout << linkLog << std::endl;

	// install the program in OpenGL pipeline
	sprog->Install();
	
	// print actives attributes
	sprog->PrintAttrib();
	sprog->PrintUniform();

	// initialize marching cube
	float minValue = 1.8f;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	minX = minY = minZ = -8.0f;
	maxX = maxY = maxZ = 8.0f;

	unsigned int nX, nY, nZ;
	nX = nY = nZ = 20;

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

	// run marching cube
	std::vector<Triangle> mcTriangles;

	MarchingCube mc;
	mc.SetMinValue(minValue);
	mc.SetDimension(nX, nY, nZ);
	mc.Process(mcPoints, mcTriangles);

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

			normalArray.push_back(t.normal.x);
			normalArray.push_back(t.normal.y);
			normalArray.push_back(t.normal.z);
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
	GLuint normalBufferHandle = vboHandles[3];

	// populate vbo
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, positionArray.size() * sizeof(float),
		 positionArray.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, colorArray.size() * sizeof(float),
		 colorArray.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, normalArray.size() * sizeof(float),
		 normalArray.data(), GL_STATIC_DRAW);

	// init vao
	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);	

	glEnableVertexAttribArray(PositionLocation);
	glEnableVertexAttribArray(ColorLocation);
	glEnableVertexAttribArray(NormalLocation);

	// map corresponding vbo to vao attribArray
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
	glVertexAttribPointer(PositionLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
	glVertexAttribPointer(ColorLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	glBindBuffer(GL_ARRAY_BUFFER, normalBufferHandle);
	glVertexAttribPointer(NormalLocation, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte*) NULL);

	/* initialisation des fonctions callback appelées par glut 
	pour respectivement le redimensionnement de la fenêtre
	et le rendu de la scène */
	glutReshapeFunc (reshape);
	glutDisplayFunc (display);

	/* entre dans la boucle d'évènements */
	glutMainLoop ();

	delete sprog;
	return 0;
}
