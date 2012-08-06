#pragma once

#include <vector>

// based on ken perlin reference implementation : improved perlin noise (2002)
class PerlinNoise
{
public:
	PerlinNoise();
	explicit PerlinNoise(int seed);

public:
	double Noise(double x, double y, double z);

private:
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);

private:
	std::vector<unsigned int> _p;
};
