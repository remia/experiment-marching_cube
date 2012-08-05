#include "MyGLWindow.h"
#include "MarchingCube.h"
#include "Utils/Chrono.h"

#include "GLSystem/GLTexture.h"

#include <iostream>

MyGLWindow::MyGLWindow()
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
	const int TextureLocation = 3;
	sprog.BindAttribLocation(PositionLocation, "VertexPosition");
	sprog.BindAttribLocation(ColorLocation, "VertexColor");
	sprog.BindAttribLocation(NormalLocation, "VertexNormal");
	sprog.BindAttribLocation(TextureLocation, "VertexTexCoord");

	std::string linkLog;
	bool lres = sprog.Link(linkLog);
	if( !lres ) std::cout << linkLog << std::endl;

	// install the program in OpenGL pipeline
	sprog.Install();
	
	// print actives attributes
	sprog.PrintAttrib();
	sprog.PrintUniform();

	// timings
	Chrono chrono;
	chrono.start();

	// initialize marching cube
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

	std::cout << "Initialization timer (ms) : " << chrono.ellapsed() << std::endl;

	// run marching cube
	chrono.start();
	std::vector<Triangle> mcTriangles;

	MarchingCube mc;
	mc.SetMinValue(minValue);
	mc.SetDimension(nX, nY, nZ);
	mc.Process(mcPoints, mcTriangles);
	
	std::cout << "Marching cube timer (ms) : " << chrono.ellapsed() << std::endl;

	unsigned int triangleCount = mcTriangles.size();
	std::cout << "Point count : " << mcPoints.size() << std::endl;
	std::cout << "Triangle count : " << triangleCount << std::endl;

	// initialize data
	std::vector<float> positionArray;
	std::vector<float> normalArray;

	for(auto t : mcTriangles)
	{
		for(int i = 0; i < 3; ++i)
		{
			positionArray.push_back(t.vertices[i].x);
			positionArray.push_back(t.vertices[i].y);
			positionArray.push_back(t.vertices[i].z);

			normalArray.push_back(t.normal[i].x);
			normalArray.push_back(t.normal[i].y);
			normalArray.push_back(t.normal[i].z);
		}
	}

	// load axis
	float axis_vertices[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	float axis_color[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};
	
	unsigned int axis_indices[] = { 0, 1, 0, 2, 0, 3 };

	GLPrimitiveT axis(new GLPrimitive3D);
	axis->SetBufferData(PositionLocation, axis_vertices, 4 * 3, 3);
	axis->SetBufferData(ColorLocation, axis_color, 4 * 3, 3);
	axis->SetIndicesBufferData(axis_indices, 6);
	axis->SetPrimitiveType(GL_LINES);
	_primitiveLibrary["origin_axis"] = axis;

	// load scene floor (textured)
	float floor_vertices[] = {
		-2.0f, 0.0f, 2.0f,
		 2.0f, 0.0f, 2.0f,
		 2.0f, 0.0f,-2.0f,
		-2.0f, 0.0f,-2.0f
	};

	float floor_textCoord[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	unsigned int floor_indices[] = { 0, 3, 1, 1, 3, 2 };

	GLTextureT floor_texture(new GLTexture);
	floor_texture->LoadFromFile("./data/texture/BlueMarblePersian-ColorMap.png");

	GLPrimitiveT floor(new GLPrimitive3D);
	floor->SetBufferData(PositionLocation, floor_vertices, 4 * 3, 3);
	floor->SetBufferData(TextureLocation, floor_textCoord, 4 * 2, 2);
	floor->SetIndicesBufferData(floor_indices, 6);
	floor->SetPrimitiveType(GL_TRIANGLES);
	floor->BindTexture(floor_texture);
	_primitiveLibrary["Floor"] = floor;

	// create 3D primitive
	GLPrimitiveT mesh(new GLPrimitive3D);
	mesh->SetBufferData(PositionLocation, positionArray, 3);
	mesh->SetBufferData(NormalLocation, normalArray, 3);
	mesh->SetPrimitiveType(GL_TRIANGLES);
	_primitiveLibrary["MCMesh"] = mesh;

	Entity3D e0("axis");
	e0.SetPrimitive(_primitiveLibrary, "origin_axis");
	e0.SetPosition( glm::vec3(0.0f) );

	Entity3D e1("mesh");
	e1.SetPrimitive(_primitiveLibrary, "MCMesh");
	e1.SetPosition( glm::vec3(0.0f, 0.0f, 10.0f) );

	Entity3D e2("mesh");
	e2.SetPrimitive(_primitiveLibrary, "MCMesh");
	e2.SetPosition( glm::vec3(4.0f, 0.0f, -10.0f) );
	
	Entity3D e3("floor");
	e3.SetPrimitive(_primitiveLibrary, "Floor");
	e3.SetPosition( glm::vec3(0.0f, -2.0f, -10.0f) );
	
	_actors.push_back(e0);
	_actors.push_back(e1);
	_actors.push_back(e2);
	_actors.push_back(e3);

	// camera settings
	_camera.SetPosition( glm::vec3(0.0f, 0.0f, 0.0f) );
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

	for(Entity3D e : _actors)
	{
		// set matrices model view proj
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), e.Position());
		glm::mat4 viewMatrix = _camera.ViewMatrix();
		glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
		glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelViewMatrix));
		glm::mat4 pmvMatrix = projectionMatrix * viewMatrix;
		pmvMatrix *= modelMatrix;

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
			glm::vec3 Intensity;// Light intensity (ads)
		};
		LightInfo Light;
		Light.Position = glm::vec4(0.0f, 5.0f, 0.0f, 1.0f);
		Light.Intensity = glm::vec3(1.0f, 1.0f, 1.0f);

		struct MaterialInfo
		{
			glm::vec3 Ka;		// Ambient reflectivity
			glm::vec3 Kd;		// Diffuse reflectivity
			glm::vec3 Ks;		// Specular reflectivity
			float Shininess;	// Specular shininess factor
		};
		MaterialInfo Material;
		Material.Ka = glm::vec3(0.3f, 0.7f, 0.9f);
		Material.Kd = glm::vec3(0.3f, 0.7f, 0.9f);
		Material.Ks = glm::vec3(1.0f, 1.0f, 1.0f);
		Material.Shininess = 30.0f;

		glUniform4fv(sprog.GetUniformLocation("Light.Position"), 1, glm::value_ptr(Light.Position));
		glUniform3fv(sprog.GetUniformLocation("Light.Intensity"), 1, glm::value_ptr(Light.Intensity));
		
		glUniform3fv(sprog.GetUniformLocation("Material.Ka"), 1, glm::value_ptr(Material.Ka));
		glUniform3fv(sprog.GetUniformLocation("Material.Kd"), 1, glm::value_ptr(Material.Kd));
		glUniform3fv(sprog.GetUniformLocation("Material.Ks"), 1, glm::value_ptr(Material.Ks));
		glUniform1f(sprog.GetUniformLocation("Material.Shininess"), Material.Shininess);

		glUniform1i(sprog.GetUniformLocation("Tex1"), 0);

		GLuint diffuseLocation = sprog.GetSubroutineIndex(GL_VERTEX_SHADER, "phongShading");
		glUniformSubroutinesuiv(GL_VERTEX_SHADER, 1, &diffuseLocation);

		std::string coloringModel;
		if(e.GetID() == "floor") coloringModel = "texturedShadingColor";
		if(e.GetID() == "axis") coloringModel = "solidColor";
		if(e.GetID() == "mesh") coloringModel = "shadingColor";

		GLuint colorModelLocation = sprog.GetSubroutineIndex(GL_FRAGMENT_SHADER, coloringModel);
		glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &colorModelLocation);		

		// draw
		e.Draw();
	}

	glFlush();
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

	_camera.SetRotationX(_camera.Rotation().x - dy * 8);
	_camera.SetRotationY(_camera.Rotation().y - dx * 8);

	lastMousePos.x = x;
	lastMousePos.y = y;
}

void MyGLWindow::keyboardEvent(unsigned char keys, int x, int y)
{
	glm::vec3 pos = _camera.Position();
	const float speed = 4.0f;

	if(keys == 'z')
		_camera.Move(-0.1f * speed);
	if(keys == 's')
		_camera.Move(0.1f * speed);
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

