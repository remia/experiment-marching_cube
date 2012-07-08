#include "MarchingCube.h"
#include "MCTable.h"

#include <iostream>

glm::vec3 intersection(glm::vec4 p1, glm::vec4 p2, float value)
{
	glm::vec3 inter;
	glm::vec3 p1v = glm::vec3(p1.x, p1.y, p1.z);
	glm::vec3 p2v = glm::vec3(p2.x, p2.y, p2.z);
	
	if(p1.w != p2.w)
		inter = p1v + (p2v - p1v) / (p2.w - p1.w) * (value - p1.w);
	else
		inter = p1v;

	return inter;
}

MarchingCube::MarchingCube()
{


}

MarchingCube::~MarchingCube()
{


}

void MarchingCube::SetMinValue(float minValue)
{
	_minValue = minValue;
}

void MarchingCube::SetDimension(unsigned int nx, unsigned int ny, unsigned int nz)
{
	_nx = nx;
	_ny = ny;
	_nz = nz;
}

void MarchingCube::Process(const std::vector<glm::vec4>& points, std::vector<Triangle>& triangles)
{
	triangles.clear();
	triangles.reserve(_nx * _ny * _nz);

	unsigned int YtimeZ = (_ny + 1) * (_nz + 1);

	for(unsigned int x = 0; x < _nx; ++x)
	{	
		for(unsigned int y = 0; y < _ny; ++y)
		{		
			for(unsigned int z = 0; z < _nz; ++z)
			{
				// value for each of the 8 vertices on the cube
				unsigned int index = x * YtimeZ + y * (_nz + 1) + z;	
				glm::vec4 cubeVertices[8];
				cubeVertices[0] = points[index];
				cubeVertices[1] = points[index + YtimeZ];
				cubeVertices[2] = points[index + YtimeZ + 1];
				cubeVertices[3] = points[index + 1];
				cubeVertices[4] = points[index + (_nz + 1)];
				cubeVertices[5] = points[index + YtimeZ + (_nz + 1)];
				cubeVertices[6] = points[index + YtimeZ + (_nz + 1) + 1];
				cubeVertices[7] = points[index + (_nz + 1) + 1];

				// index in the LUT based on which vertices are inside
				// 256 cases
				int cubeIndex = 0;
				for(unsigned int n = 0; n < 8; ++n)
				{
					// if vertice n inside (<= minValue)
					if(cubeVertices[n].w <= _minValue)
						cubeIndex |= (1 << n);
				}

				// early exit if cube completly inside or outside
				if( !edgeTable[cubeIndex] )
					continue;

				// get intersections points (vertices) on the edges of the cube
				// 12 edges
				glm::vec3 intersectPoints[12];
				if(edgeTable[cubeIndex] & 1) 
					intersectPoints[0] = intersection(cubeVertices[0], cubeVertices[1], _minValue);
				if(edgeTable[cubeIndex] & 2) 
					intersectPoints[1] = intersection(cubeVertices[1], cubeVertices[2], _minValue);
				if(edgeTable[cubeIndex] & 4) 
					intersectPoints[2] = intersection(cubeVertices[2], cubeVertices[3], _minValue);
				if(edgeTable[cubeIndex] & 8) 
					intersectPoints[3] = intersection(cubeVertices[3], cubeVertices[0], _minValue);
				if(edgeTable[cubeIndex] & 16) 
					intersectPoints[4] = intersection(cubeVertices[4], cubeVertices[5], _minValue);
				if(edgeTable[cubeIndex] & 32) 
					intersectPoints[5] = intersection(cubeVertices[5], cubeVertices[6], _minValue);
				if(edgeTable[cubeIndex] & 64) 
					intersectPoints[6] = intersection(cubeVertices[6], cubeVertices[7], _minValue);
				if(edgeTable[cubeIndex] & 128) 
					intersectPoints[7] = intersection(cubeVertices[7], cubeVertices[4], _minValue);
				if(edgeTable[cubeIndex] & 256) 
					intersectPoints[8] = intersection(cubeVertices[0], cubeVertices[4], _minValue);
				if(edgeTable[cubeIndex] & 512) 
					intersectPoints[9] = intersection(cubeVertices[1], cubeVertices[5], _minValue);
				if(edgeTable[cubeIndex] & 1024) 
					intersectPoints[10] = intersection(cubeVertices[2], cubeVertices[6], _minValue);
				if(edgeTable[cubeIndex] & 2048) 
					intersectPoints[11] = intersection(cubeVertices[3], cubeVertices[7], _minValue);

				// build the triangles using 2nd LUT
				for(int n = 0; triTable[cubeIndex][n] != -1; n+=3)
				{
					Triangle t;
					t.vertices[0] = intersectPoints[triTable[cubeIndex][n+2]];
					t.vertices[1] = intersectPoints[triTable[cubeIndex][n+1]];
					t.vertices[2] = intersectPoints[triTable[cubeIndex][n]];
					
					// compute normal approx.
					const glm::vec3& a = glm::vec3(t.vertices[0]);
					const glm::vec3& b = glm::vec3(t.vertices[1]);
					const glm::vec3& c = glm::vec3(t.vertices[2]);
					t.normal = glm::normalize(glm::cross(c - a, b - a));
					
					triangles.push_back(t);
				}

			}
		}
	}

}
