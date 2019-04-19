#ifndef WORLD_H
#define WORLD_H
#include <vector>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "Chunk.h"
#include "render_pass.h"
using namespace glm;

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

	//PLAN: Have this check if the movement result in collision in x, y, or z direction and zero's out that
	//component on return if there is a collision in that direction
	vec3 checkCollision(vec3 & p, vec3& move) {
		vec3 pos = vec3(float((int)p.x / 50) * 50.0f - 50.0f, 0, float((int)p.z / 50) * 50.0f  - 50.0f);
		int chunkId = -1;
		vec3 toRet(1.0, 1.0, 1.0);
		for (unsigned int i = 0; i < chunks.size(); i++)
		{
			vec2 wPos(chunks[i]->worldPos.x + 25.0, chunks[i]->worldPos.z + 25.0);
			if (distance(wPos, vec2(p.x, p.z)) < 150.0)
			{
				//Different Method
				for (unsigned int l = 0; l < 50; l++)
				{
					for (unsigned int j = 0; j < chunks[i]->blocks[l]->cube_location.size(); j++)
					{
						vec3* focus = &chunks[i]->blocks[l]->cube_location[j];
						vec3 pers = vec3(p.x, p.y, p.z);
						vec3 ray = (pers + move) - *focus - vec3(0.5, 1.0, 0.5);

						if (abs(ray.x) <= 0.6 && ray.y < 1.75 && abs(ray.z) <= 0.6) {
							toRet.y = 0.0;
							if (ray.y <= 1.75) {
								canJump = true;
							}
							if (!(ray.y <= 1.3))
							{
								continue;
							}
							toRet.x = 0.0;
							toRet.z = 0.0;
						}

					}
				}
			}
			
		}
		return toRet;
	}

	vec3 checkPlayer(vec3& p, vec3& move, bool jump, bool god) {
		if (!god)
		{
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
					move.y = move.y + grav;
				}
			}
		}
		
		return checkCollision(p, move);

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
			if (distance(pos, vec3(chunks[i]->worldPos)) > 300.0)
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
		for (int i = -3; i <= 2; i++)
		{
			for (int j = -3; j <= 2; j++)
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