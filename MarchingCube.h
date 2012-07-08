#pragma once

#include <glm/glm.hpp>
#include <vector>

struct Triangle
{
	glm::vec3 vertices[3];
	glm::vec3 normal;
};

class MarchingCube
{
public:
	MarchingCube();
	~MarchingCube();

public:
	void Process(const std::vector<glm::vec4>& points, std::vector<Triangle>& triangles);
	
	void SetMinValue(float minValue);
	void SetDimension(unsigned int nx, unsigned int ny, unsigned int nz);

private:
	unsigned int _nx; 
	unsigned int _ny;
	unsigned int _nz;

	float _minValue;
};
