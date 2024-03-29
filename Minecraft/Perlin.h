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

	//Will take in world position as the seed for the continuity
	vec2 seed;

	int width, height;

	float perlin(float x, float y) {
		int x0 = (int)x;
		int y0 = (int)y;
		int x1 = x0 + 1;
		int y1 = y0 + 1;
		float w0 = x - float(x0);
		float w1 = y - float(y0);

		float v00, v01, v10, v11, inter0, inter1, toRet;

		v00 = sampleGradient(x0, y0, x, y);
		v01 = sampleGradient(x1, y0, x, y);
		v10 = sampleGradient(x0, y1, x, y);
		v11 = sampleGradient(x1, y1, x, y);

		inter0 = blend(v00, v01, w0);
		inter1 = blend(v10, v11, w0);

		toRet = blend(inter0, inter1, w1);
		return toRet;
	}

	Perlin(vec4 wPos, int width, int height, int fullWidth = 0, int fullHeight = 0) {
		if (fullHeight == 0) fullHeight = seed_height;
		if (fullWidth == 0) fullWidth = seed_width;
		seed = vec2(wPos.x, wPos.z);
		srand(wPos.x * 0.444 + wPos.z * 0.444);
		this->width = width + 1;
		this->height = height + 1;
		for (int i = 0; i < height + 1; i++)
		{
			for (int j = 0; j < width + 1; j++)
			{
				if (i == 0 || j == 0 || i == height || j == width)
				{
					float x = wPos.x + (float)j / width * fullWidth;
					float y = wPos.z + (float)i / height * fullHeight;
					perl.push_back(normalize(vec2(sin((x + 0.5) * 0.8144), cos((y + 0.5) * 0.3456))));
					
				}
				else {
					float x = wPos.x + (float)j / width * fullWidth;
					float y = wPos.z + (float)i / height * fullHeight;
					perl.push_back(normalize(vec2(float(rand()) / RAND_MAX, float(rand()) / RAND_MAX)));
					//addto(perl);
				}
			}
		}
		for (int i = 0; i < fullHeight; i++)
		{
			for (int j = 0; j < fullWidth; j++)
			{
				end.push_back(10.0f * perlin((float)j / (float)(fullHeight) * (float)width, (float)i / (float)(fullWidth) * (float)height));
			}
		}
	}
private:
	void addto(std::vector<vec2>& v) {
		v.push_back(normalize(vec2((double)rand() / (RAND_MAX), (double)rand() / (RAND_MAX))));
	}
	
	float blend(float x, float y, float w) {
		return x * (1.0 - w) + y * w;
	}

	float sampleGradient(int x, int y, float interX, float interY) {
		vec2 big = vec2(interX - float(x), interY - float(y));
		return dot(big, perl[y+ x * width]);
	}
};

#endif