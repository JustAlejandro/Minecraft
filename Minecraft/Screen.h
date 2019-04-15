#ifndef SCREEN_H
#define SCREEN_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
using namespace glm;
//Generates the quad to render the texture on screen
struct Screen
{
public:
	Screen();
	void toScreen(GLuint& mainRenderTex, int& width, int& height);

	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_uv;
	std::vector<uvec3> quad_faces;
private:
	RenderDataInput input;
	RenderPass* render;
	const char* screen_frag =
#include "screen.frag"
		; 
	const char* screen_vert =
#include "screen.vert"
		;
};

#endif