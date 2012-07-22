#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_precision.hpp>
#include <vector>

struct Triangle
{
	glm::vec3 vertices[3];
	glm::vec3 normal[3];
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

protected:
	unsigned int getPointIndex(const glm::i8vec3& pos);

	glm::vec4 vertexGradient(
		const glm::i8vec3& pos,
		const std::vector<glm::vec4>& points);
	
	glm::vec3 interpolatedGradient(
		const glm::i8vec3& posA, const glm::i8vec3& posB,
		const std::vector<glm::vec4>& points);

	bool vertexOnBoundaries(const glm::i8vec3& pos);

private:
	unsigned int _nx; 
	unsigned int _ny;
	unsigned int _nz;

	float _minValue;
};
