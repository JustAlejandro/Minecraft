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
	int deleteDelay = 0;

	World(MatrixPointers* mat, vec4* lightIn) {
		startUp(mat, lightIn);
	}

	void checkCollision(vec3 & p) {
		vec3 pos = vec3(float((int)p.x / 50) * 50.0f - 50.0f, 0, float((int)p.z / 50) * 50.0f  - 50.0f);
		int chunkId = -1;
		for (unsigned int i = 0; i < chunks.size(); i++)
		{
			vec4 focus = chunks[i]->worldPos;
			if (focus.x < pos.x + 1.0 && focus.x > pos.x - 1.0) {
				if (focus.z < pos.z + 1.0 && focus.z > pos.z - 1.0) {
					chunkId = i;
				}
			}

		}
		if (chunkId == -1) return;
		for (unsigned int i = 0; i < 50; i++)
		{
			if (chunks[chunkId]->blocks[i]->cube_location.size() == 0) continue;
			vec3 look = chunks[chunkId]->blocks[i]->cube_location[0];
			look.x = look.x + 0.5;
			look.z = look.z + 0.5;
			look.y = look.y + 1.0;
			//check x coord
			int dirx = 0;
			int dirz = 0;
			int diry = 0;
			if (abs(look.x - p.x) < 1.0) {
				if (look.x - p.x < 1.0 && look.x - p.x > 0.0)
					dirx = 1;
				else if (look.x - p.x > -1.0 && look.x - p.x < 0.0) 
					dirx = -1;
				for (unsigned int j = 0; j < chunks[chunkId]->blocks[i]->cube_location.size(); j++)
				{
					vec3 focus = chunks[chunkId]->blocks[i]->cube_location[j];
					focus.z = focus.z + 0.5;
					dirz = 0;
					if (p.y - focus.y - 1.75 < 0.0)
					{
						p.x = look.x - 0.5 * dirx;
					}
					else
					{
						p.y = focus.y + 1.75;
					}
				}
					
			}
			for (unsigned int j = 0; j < chunks[chunkId]->blocks[i]->cube_location.size(); j++)
			{
				vec3 focus = chunks[chunkId]->blocks[i]->cube_location[j];
				focus.z = focus.z + 0.5;
				dirz = 0;
				if (p.y - focus.y - 1.75 < 0.0)
				{
					if (abs(focus.z - p.z) < 1.0 && p.y - focus.y - 1.75 < -0.1)
					{
						if (focus.z - p.z < 1.0 && focus.z - p.z > 0.0)
							dirz = 1;
						else if (focus.z - p.z > -1.0 && focus.z - p.z < 0.0)
							dirz = -1;
						p.z = focus.z - 0.5 * dirz;

					}
					else
					{
						p.y = focus.y + 1.75;
					}
				}

			}
		}
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
		checkCollision(p);

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
				if (deleteDelay <= 0)
				{
					chunks.erase(chunks.begin() + i);
					deleteDelay = 30;
				}
				else
				{
					deleteDelay--;
				}
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
					if (chunks.size() > 0) return;
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