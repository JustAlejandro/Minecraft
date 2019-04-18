#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "Chunk.h"
#include "render_pass.h"

class World
{
public:
	World(MatrixPointers* mat, vec4* lightIn) {
		startUp(mat, lightIn);
	}
	std::vector<Chunk*> chunks;
	RenderPass* render;
	RenderDataInput* input;
private:
	void startUp(MatrixPointers* mat, vec4* lightIn) {
		if (Cubes::cube_vert.size() == 0)
			Cubes::setup(mat, lightIn);
	}
};



#endif