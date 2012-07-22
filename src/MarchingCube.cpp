#include "MarchingCube.h"
#include "MCTable.h"

#include <iostream>

// linear interpolation between p1 and p2
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

unsigned int MarchingCube::getPointIndex(const glm::i8vec3& pos)
{
	unsigned int YtimeZ = (_ny + 1) * (_nz + 1);
	return pos.x * YtimeZ + pos.y * (_nz + 1) + pos.z;
}

glm::vec4 MarchingCube::vertexGradient(
	const glm::i8vec3& pos,
	const std::vector<glm::vec4>& points)
{
	const unsigned int x = getPointIndex(pos);

	const unsigned int xprev = getPointIndex(pos - glm::i8vec3(1, 0, 0));
	const unsigned int xnext = getPointIndex(pos + glm::i8vec3(1, 0, 0));

	const unsigned int yprev = getPointIndex(pos - glm::i8vec3(0, 1, 0));
	const unsigned int ynext = getPointIndex(pos + glm::i8vec3(0, 1, 0));
	
	const unsigned int zprev = getPointIndex(pos - glm::i8vec3(0, 0, 1));
	const unsigned int znext = getPointIndex(pos + glm::i8vec3(0, 0, 1));
	
	glm::vec4 gradient(
		points[xnext].w - points[xprev].w,
		points[ynext].w - points[yprev].w,
		points[znext].w - points[zprev].w,
		points[x].w
	);

	// scale factor
	float factor = 1.0f;
	return gradient * glm::vec4(factor, factor, factor, 1.0f);
	
}

bool MarchingCube::vertexOnBoundaries(const glm::i8vec3& pos)
{
	return !( pos.x > 0 && pos.y > 0 && pos.z > 0
			&& pos.x < _nx && pos.y < _ny && pos.z < _nz );
}

glm::vec3 MarchingCube::interpolatedGradient(
	const glm::i8vec3& posA, const glm::i8vec3& posB,
	const std::vector<glm::vec4>& points)
{
	// check if on boundaries
	glm::vec4 gradientA(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec4 gradientB(1.0f, 1.0f, 1.0f, 1.0f);

	if( !vertexOnBoundaries(posA) )	
		gradientA = vertexGradient(posA, points);
	if( !vertexOnBoundaries(posB) )	
		gradientB = vertexGradient(posB, points);

	return intersection(gradientA, gradientB, _minValue);
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
				/* index on the cube :
				
				   7 +------+ 6 
				    /      /|
				 4 +------+5|
				 3 | +    | + 2
				   |      |/
				 0 +------+ 1 

				*/

				// value for each of the 8 vertices on the cube
				glm::i8vec3 pos(x, y, z);
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
				glm::vec3 gradients[12];

				if(edgeTable[cubeIndex] & 1) 
				{
					intersectPoints[0] = intersection(cubeVertices[0], cubeVertices[1], _minValue);
					
					glm::i8vec3 posA = pos;
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 0.0f, 0.0f);
					gradients[0] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 2) 
				{				
					intersectPoints[1] = intersection(cubeVertices[1], cubeVertices[2], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 0.0f, 0.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 0.0f, 1.0f);
					gradients[1] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 4) 
				{
					intersectPoints[2] = intersection(cubeVertices[2], cubeVertices[3], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 0.0f, 1.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(0.0f, 0.0f, 1.0f);
					gradients[2] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 8) 
				{	
					intersectPoints[3] = intersection(cubeVertices[3], cubeVertices[0], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(0.0f, 0.0f, 1.0f);
					glm::i8vec3 posB = pos;
					gradients[3] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 16) 
				{
					intersectPoints[4] = intersection(cubeVertices[4], cubeVertices[5], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(0.0f, 1.0f, 0.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 1.0f, 0.0f);
					gradients[4] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 32) 
				{
					intersectPoints[5] = intersection(cubeVertices[5], cubeVertices[6], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 1.0f, 0.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 1.0f, 1.0f);
					gradients[5] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 64) 
				{
					intersectPoints[6] = intersection(cubeVertices[6], cubeVertices[7], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 1.0f, 1.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(0.0f, 1.0f, 1.0f);
					gradients[6] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 128) 
				{
					intersectPoints[7] = intersection(cubeVertices[7], cubeVertices[4], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(0.0f, 1.0f, 1.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(0.0f, 1.0f, 0.0f);
					gradients[7] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 256) 
				{
					intersectPoints[8] = intersection(cubeVertices[0], cubeVertices[4], _minValue);
					
					glm::i8vec3 posA = pos;
					glm::i8vec3 posB = pos + glm::i8vec3(0.0f, 1.0f, 0.0f);
					gradients[8] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 512) 
				{
					intersectPoints[9] = intersection(cubeVertices[1], cubeVertices[5], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 0.0f, 0.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 1.0f, 0.0f);
					gradients[9] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 1024) 
				{
					intersectPoints[10] = intersection(cubeVertices[2], cubeVertices[6], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(1.0f, 0.0f, 1.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(1.0f, 1.0f, 1.0f);
					gradients[10] = interpolatedGradient(posA, posB, points);
				}
				if(edgeTable[cubeIndex] & 2048) 
				{
					intersectPoints[11] = intersection(cubeVertices[3], cubeVertices[7], _minValue);
					
					glm::i8vec3 posA = pos + glm::i8vec3(0.0f, 0.0f, 1.0f);
					glm::i8vec3 posB = pos + glm::i8vec3(0.0f, 1.0f, 1.0f);
					gradients[11] = interpolatedGradient(posA, posB, points);
				}

				// build the triangles using 2nd LUT
				for(int n = 0; triTable[cubeIndex][n] != -1; n+=3)
				{
					Triangle t;
					
					unsigned int triIndex[3] = {
						triTable[cubeIndex][n+2],
						triTable[cubeIndex][n+1],
						triTable[cubeIndex][n]
					};
					for(int v = 0; v < 3; ++v)
					{
						t.vertices[v] = intersectPoints[triIndex[v]];
						t.normal[v] = glm::normalize(gradients[triIndex[v]]);
					}
					//// compute normal approx.
					//const glm::vec3& a = glm::vec3(t.vertices[0]);
					//const glm::vec3& b = glm::vec3(t.vertices[1]);
					//const glm::vec3& c = glm::vec3(t.vertices[2]);
					//t.normal = glm::normalize(glm::cross(c - a, b - a));

					triangles.push_back(t);
				}
			}
		}
	}

}
