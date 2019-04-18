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
	//Rate of fall
	float grav = -0.2;
	bool canJump = true;
	int jumpFrames = 0;

	World(MatrixPointers* mat, vec4* lightIn) {
		startUp(mat, lightIn);
	}

	void checkPlayer(vec3& p, bool jump) {
		if (jump && canJump)
		{
			canJump = false;
			p.y = p.y - grav;
			jumpFrames = 12;
		}
		else
		{
			if (jumpFrames != 0)
			{
				jumpFrames--;
				p.y = p.y - grav;
			}
			else
			{
				p.y = p.y + grav;
			}
		}
		if (p.y < 10.0)
		{
			canJump = true;
			p.y = 10.0;
		}
		//checkCollision(p);

	}
	bool checkExist(std::vector<Chunk*>& c, vec3 pos) {
		int j;
		for (unsigned int i = 0; i < c.size(); i++)
		{
			vec4 focus = c[i]->worldPos;
			if (focus.x < pos.x + 1.0 && focus.x > pos.x - 1.0) {
				if (focus.z < pos.z + 1.0 && focus.z > pos.z - 1.0) {
					return true;
				}
			}
				
		}
		return false;
	}

	void cull(vec3 pos) {
		for (unsigned int i = 0; i < chunks.size(); i++)
		{
			if (distance(pos, vec3(chunks[i]->worldPos)) > 170.0)
			{
				chunks.erase(chunks.begin() + i);
				return;
			}
		}
	}

	void worldUpdate(vec3 playerPos, MatrixPointers* mats, vec4* light) {
		vec3 roundedDown = vec3(float((int)playerPos.x / 50) * 50.0f, 0, float((int)playerPos.z / 50) * 50.0f);
		for (int i = -2; i <= 1; i++)
		{
			for (int j = -2; j <= 1; j++)
			{
				if (!checkExist(chunks, vec3(roundedDown.x + i * seed_height, 0.0, roundedDown.z + j * seed_width))) {
					chunks.push_back(new Chunk(mats, light, vec4(roundedDown.x + i * seed_height, 0.0, roundedDown.z + j * seed_width, 1.0), 12, 0));
				}
			}

		}
		cull(roundedDown);
	}

	void toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height) {
		for (unsigned int i = 0; i < chunks.size(); i++)
		{
			chunks[i]->toScreen(FrameBuffer, mat, l, width, height);
		}
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