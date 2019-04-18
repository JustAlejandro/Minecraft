#ifndef PERLIN_H
#define PERLIN_H
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
using namespace glm;

const int seed_width = 50;
const int seed_height = 50;

class Perlin {
public:
	std::vector<vec2> perl;

	std::vector<float> end;

	//Will take in world position as the seed
	vec2 seed;

	int width, height;

	float perlin(float x, float y) {
		int x0 = (int)x;
		int y0 = (int)y;
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		float w0 = x - float(x0);
		float w1 = y - float(y0);

		float v0, v1, inter0, inter1, toRet;

		v0 = sampleGradient(x0, y0, x, y);
		v1 = sampleGradient(x1, y0, x, y);
		inter0 = lerp(v0, v1, w0);

		v0 = sampleGradient(x0, y1, x, y);
		v1 = sampleGradient(x1, y1, x, y);
		inter1 = lerp(v0, v1, w0);

		toRet = lerp(inter0, inter1, w1);
		return toRet;
	}

	Perlin(vec4 wPos, int width, int height) {
		seed = vec2(wPos.x, wPos.z);
		srand(wPos.x * 0.444 + wPos.z * 0.444);
		this->width = width + 1;
		this->height = height + 1;
		for (int i = 0; i < height + 1; i++)
		{
			for (int j = 0; j < width + 1; j++)
			{
				
				{
					addto(perl);
				}
			}
		}

		for (int i = 0; i < seed_height; i++)
		{
			for (int j = 0; j < seed_width; j++)
			{
				end.push_back(4.0f * perlin((float)i / (float)seed_height * (float)width, (float)j / (float)seed_width * (float)height));
			}
		}
	}
private:
	void addto(std::vector<vec2>& v) {
		v.push_back(normalize(vec2((double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX))));
	}
	
	float lerp(float x, float y, float w) {
		return x * (1.0 - w) + y * w;
	}

	float sampleGradient(int x, int y, float interX, float interY) {
		vec2 big = vec2(interX - float(x), interY - float(y));
		return dot(big, perl[y * width + x]);
	}
};

#endif