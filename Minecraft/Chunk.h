#ifndef CHUNK_H
#define CHUNK_H
#include <vector>
#include <GL/glew.h>
#include "render_pass.h"
#include "GUI.h"
#include "Cube.h"
#include "Perlin.h"
using namespace glm;

struct Chunk
{
public:
	Chunk(MatrixPointers* mats, vec4* light, vec4 w_pos, int max, int min);
	~Chunk();
	void toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height);
	Cubes* blocks[seed_height];
	vec4 worldPos;
	int width;
	int height;
private:
	Perlin frank;
	Perlin half;
	Perlin quarter;
	std::vector<float> map;
	void getMinMax(std::vector<float> map, int & min, int & max, int& x, int& y, float big, float small, int maxV, int minV);
	void placeBlocks(int min, int max, int x, int y);
	void setupMap();
};



#endif