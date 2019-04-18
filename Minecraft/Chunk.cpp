#include "Chunk.h"
#include <algorithm>


int reMap(float val, float h, float l, int max, int min) {
	return min + (int)((val - l) * ((float)max - (float)min) / (h - l));
}

// Takes in a 2d map of the chunk to create, as well as it's world position and the max and min values
Chunk::Chunk(MatrixPointers* mats, vec4* light, vec4 w_pos, int max, int min) : frank(w_pos, 5, 5), half(w_pos, 10, 10), quarter(w_pos, 15,15)
{
	float big = 1.0f;
	float small = -1.0f;
	worldPos = w_pos;
	width = seed_width;
	height = seed_height;
	setupMap();
	int minBlocks;
	int maxBlocks;
	for (int i = 0; i < height; i++)
	{
		blocks[i] = new Cubes(mats, light);
		for (int j = 0; j < width; j++)
		{
			minBlocks = 100000;
			maxBlocks = -1;
			getMinMax(map, minBlocks, maxBlocks, i , j, big, small, max, min);
			placeBlocks(minBlocks, maxBlocks, i, j);
		}
	}
	CHECK_GL_ERROR(glGenTextures(1, &tex));
	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, tex));
	CHECK_GL_ERROR(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, seed_width, seed_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, half.end.data()));
}

Chunk::~Chunk()
{
	for (int i = 0; i < seed_height; i++)
	{
		delete blocks[i];
	}
}

//Get MinMax will find the minimum and maximum required height for a given cell, aka, lowest and max height
void Chunk::getMinMax(std::vector<float> map, int & min, int & max, int& x, int& y, float big, float small, int maxV, int minV)
{
	for (int i = -1; i <= 1; i++)
	{
		if (x + i >= width || x + i < 0) continue;
		for (int j = -1; j <= 1; j++)
		{
			if (y + j >= height || y + j < 0) continue;
			int reMappedV = reMap(map[(i + x) * seed_width + j + y], big, small, maxV, minV);
			min = std::min(min, reMappedV);
		}

	}

	max = std::max(max, reMap(map[x * seed_width + y], big, small, maxV, minV));
}

//Slap City all the blocks down at x, y, with height from min->max
void Chunk::placeBlocks(int min, int max, int x, int y) {
	for (int i = min; i <= max; i++)
	{
		(*blocks[x]).add(worldPos + vec4((float)x, (float)i, (float)y, 0.0f));
	}
}


void Chunk::toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height) {
	glBindTexture(GL_TEXTURE_2D, tex);
	for (unsigned int i = 0; i < seed_height; i++)
	{
		//TODO: This is taking all our CPU time with handing off uniforms, so we'll render our cubes a row at a time now
		Cubes::setLocation(blocks[i]->cube_location);
		(*blocks[i]).toScreen(FrameBuffer, mat, l, width, height);
	}
}

void Chunk::setupMap() {
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			map.push_back(1.0 / 2.0 * frank.end[i * seed_width + j] + 1.0 / 6.0 * half.end[i * seed_width + j] + 1.0 / 12.0 * quarter.end[i * seed_width + j]);
		}
	}
}