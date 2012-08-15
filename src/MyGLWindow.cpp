#include "MyGLWindow.h"
#include "MarchingCube.h"
#include "Utils/Chrono.h"
#include "Utils/PerlinNoise.h"

#include "GLRenderer/ShaderProgram.h"
#include "GLRenderer/ShaderLocation.h"
#include "GLRenderer/Actor.h"
#include "GLRenderer/Mesh.h"
#include "GLRenderer/ModelLoader.h"
#include "GLRenderer/Texture.h"
#include "GLRenderer/Utils.h"

#include <iostream>

// no input abstraction implemented
#include <GL/glut.h>

using namespace GLRenderer;

MyGLWindow::MyGLWindow()
{

}

MyGLWindow::~MyGLWindow()
{

}

void MyGLWindow::initialize()
{
	// load shaders
	ShaderProgramT sprog(new ShaderProgram);
	std::string vertexLog;
	bool vres = sprog->AddShaderFromFile(GL_VERTEX_SHADER,"./src/vertex.glsl", vertexLog);
	if( !vres ) std::cout << vertexLog << std::endl;

	std::string fragmentLog;
	bool fres = sprog->AddShaderFromFile(GL_FRAGMENT_SHADER, "./src/fragment.glsl", fragmentLog);
	if( !fres ) std::cout << fragmentLog << std::endl;

	sprog->BindAttribLocation(ShaderLocation::Position, "VertexPosition");
	sprog->BindAttribLocation(ShaderLocation::Color, "VertexColor");
	sprog->BindAttribLocation(ShaderLocation::Normal, "VertexNormal");
	sprog->BindAttribLocation(ShaderLocation::TexCoord, "VertexTexCoord");

	std::string linkLog;
	bool lres = sprog->Link(linkLog);
	if( !lres ) std::cout << linkLog << std::endl;

	sprog->PrintAttrib();
	sprog->PrintUniform();

	_shaderProgLib["defaultShader"] = sprog;

	// init objets
	InitMCMesh();
	InitAxis();
	InitFloor();
	InitModels();

	// add entity
	Entity axis;
	axis.SetActor(_actorLib["Axis"]);
	axis.SetPosition( glm::vec3(0.0f) );
	_scene.AddEntity(axis);

	Entity terrain;
	terrain.SetActor(_actorLib["Terrain"]);
	terrain.SetPosition( glm::vec3(0.0f, -5.0f, 10.0f) );
	terrain.SetScale( glm::vec3(5.0f, 5.0f, 5.0f) );
	_scene.AddEntity(terrain);
	
	Entity floor;
	floor.SetActor(_actorLib["Floor"]);
	floor.SetPosition( glm::vec3(0.0f, -2.0f, -10.0f) );
	_scene.AddEntity(floor);
	
	Entity tank;
	tank.SetActor(_actorLib["Tank"]);
	tank.SetPosition( glm::vec3(0.0f, 2.0f, 0.0f) );
	tank.SetScale( glm::vec3(0.01f, 0.01f, 0.01f) );
	_scene.AddEntity(tank);

	Entity dragon;
	dragon.SetActor(_actorLib["Dragon"]);
	dragon.SetPosition( glm::vec3(0.0f, 2.0f, 0.0f) );
	dragon.SetScale( glm::vec3(10.0f, 10.0f, 10.0f) );
	_scene.AddEntity(dragon);

	// camera settings
	Camera& camera = _scene.GetCamera();
	camera.SetPosition( glm::vec3(0.0f, 0.0f, 0.0f) );

	_scene.Init();
	
	Utils::CheckErrorAndPrint("MyGLWindow::InitializeGL");
}

void MyGLWindow::resize(int width, int height)
{
	_scene.Resize(width, height);
		Utils::CheckErrorAndPrint("AfterResize");
}

void MyGLWindow::paint()
{
	glEnable(GL_DEPTH_TEST);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	// draw wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	_scene.Update(0.0f);
	_scene.Draw();

	// check errors
	Utils::CheckErrorAndPrint("MyGLWindow::paint");

	glFlush();
}

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

	Camera& camera = _scene.GetCamera();
	camera.SetRotationX(camera.Rotation().x - dy * 8);
	camera.SetRotationY(camera.Rotation().y - dx * 8);

	lastMousePos.x = x;
	lastMousePos.y = y;
}

void MyGLWindow::keyboardEvent(unsigned char keys, int x, int y)
{
	const float speed = 4.0f;

	Camera& camera = _scene.GetCamera();

	if(keys == 'z')
		camera.Move(-0.1f * speed);
	if(keys == 's')
		camera.Move(0.1f * speed);
}

void MyGLWindow::InitMCMesh()
{
	// timings
	Chrono chrono;
	chrono.start();

	// initialize marching cube
	float minValue = 0; //0.5f;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;
	minX = minY = minZ = -1.5f;
	maxX = maxY = maxZ = 1.5f;

	unsigned int nX, nY, nZ;
	nX = nY = 200; nZ = 100; // 700, 700, 400 => OK

	std::vector<glm::vec4> mcPoints((nX + 1) * (nY + 1) * (nZ + 1));
	glm::vec3 mcStep((maxX - minX) / nX, (maxY - minY) / nY, (maxZ - minZ) / nZ);

	PerlinNoise perlin( time(NULL) );

	#pragma omp parallel for
	for(int x = 0; x < nX + 1; ++x)
		for(int y = 0; y < nY + 1; ++y)
			for(int z = 0; z < nZ + 1; ++z)
			{
				glm::vec4 point(
					minX + x * mcStep.x,
					minY + y * mcStep.y,
					minZ + z * mcStep.z, 0);

				// test terrain	
				point.w = -point.y;

				float freq = 11.03f;
				float amp = 0.25f;
				float sum = 0.0f;
				float lacunarity = 0.198f;
				float gain = 2.0f;
				unsigned int octaves = 9;
				
				for(unsigned int i = 0; i < octaves; ++i)
				{
					sum += perlin.Noise(point.x * freq, point.y * freq, point.z * freq) * amp;
					freq *= lacunarity;
					amp * gain;
				}

				point.w += sum;
				
				//point.w = Potential(point.x, point.y, point.z);				
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

	// create 3D primitive from marching cube
	MeshT mcMesh(new Mesh);
	mcMesh->SetBufferData(ShaderLocation::Position, positionArray, 3);
	mcMesh->SetBufferData(ShaderLocation::Normal, normalArray, 3);
	mcMesh->SetPrimitiveType(GL_TRIANGLES);
	_meshLib["MCMesh"] = mcMesh;

	Material terrainMaterial;
	terrainMaterial.SetAmbientColor( glm::vec3(0.2f, 0.9f, 0.3f) );
	terrainMaterial.SetDiffuseColor( glm::vec3(0.2f, 0.9f, 0.3f) );
	terrainMaterial.SetSpecularColor( glm::vec3(0.8f, 0.8f, 0.8f) );

	ActorT terrain(new Actor);
	terrain->AddDrawableMesh(_meshLib["MCMesh"], terrainMaterial);
	terrain->SetShaderProgram(_shaderProgLib["defaultShader"]);
	_actorLib["Terrain"] = terrain;
}

void MyGLWindow::InitAxis()
{
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

	MeshT axisMesh(new Mesh);
	axisMesh->SetBufferData(ShaderLocation::Position, axis_vertices, 4 * 3, 3);
	axisMesh->SetBufferData(ShaderLocation::Color, axis_color, 4 * 3, 3);
	axisMesh->SetIndicesBufferData(axis_indices, 6);
	axisMesh->SetPrimitiveType(GL_LINES);

	Material axisMat;
	axisMat.SetShadingMode(Material::SHADING_SOLID);

	ActorT originAxis(new Actor);
	originAxis->AddDrawableMesh(axisMesh, axisMat);
	originAxis->SetShaderProgram(_shaderProgLib["defaultShader"]);
	_actorLib["Axis"] = originAxis;
}

void MyGLWindow::InitFloor()
{
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

	TextureT floorTexture(new Texture);
	floorTexture->LoadFromFile("./data/texture/BlueMarblePersian-ColorMap.png");
	_textureLib["BlueMarble"] = floorTexture;

	MeshT floorMesh(new Mesh);
	floorMesh->SetBufferData(ShaderLocation::Position, floor_vertices, 4 * 3, 3);
	floorMesh->SetBufferData(ShaderLocation::TexCoord, floor_textCoord, 4 * 2, 2);
	floorMesh->SetIndicesBufferData(floor_indices, 6);
	floorMesh->SetPrimitiveType(GL_TRIANGLES);

	Material floorMaterial;
	floorMaterial.SetAmbientColor( glm::vec3(0.2f, 0.9f, 0.3f) );
	floorMaterial.SetDiffuseColor( glm::vec3(0.2f, 0.9f, 0.3f) );
	floorMaterial.SetSpecularColor( glm::vec3(0.8f, 0.8f, 0.8f) );
	floorMaterial.AddTexture(floorTexture);

	ActorT floor(new Actor);
	floor->AddDrawableMesh(floorMesh, floorMaterial);
	floor->SetShaderProgram(_shaderProgLib["defaultShader"]);
	_actorLib["Floor"] = floor;

}

void MyGLWindow::InitModels()
{
	Chrono chrono;

	// load models
	ModelLoader modelLoader;

	chrono.start();
	_actorLib["Tank"] = modelLoader.LoadFromFile("./data/model/stankbot/models/stankbot.dae");
	_actorLib["Tank"]->SetShaderProgram(_shaderProgLib["defaultShader"]);
	std::cout << "Load stankbot (ms) : " << chrono.ellapsed() << std::endl;

	chrono.start();
	_actorLib["Dragon"] = modelLoader.LoadFromFile("./data/model/dragon_vrip_res3.ply");
	_actorLib["Dragon"]->SetShaderProgram(_shaderProgLib["defaultShader"]);
	std::cout << "Load dragon (ms) : " << chrono.ellapsed() << std::endl;
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

