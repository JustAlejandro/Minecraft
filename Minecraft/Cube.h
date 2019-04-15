#ifndef CUBE_H
#define CUBE_H
#include <vector>
#include <GL/glew.h>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include "render_pass.h"
#include "GUI.h"
using namespace glm;

//Generates the quad to render the texture on screen
struct Cubes
{
public:
	Cubes(const mat4* viewIn, const mat4* projIn, const vec4* lightIn);
	void add(vec4 loc);
	void toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height);

	std::vector<vec4> cube_vert;
	std::vector<vec2> cube_uv;
	std::vector<uvec3> cube_faces;

	std::vector<vec4> cube_location;
	int count = 0;

private:
	RenderDataInput input;
	RenderPass* render;
	mat4 view;
	mat4 proj;
	vec4 light;
	
	const char* cubes_frag =
#include "cube.frag"
		;
	const char* cubes_vert =
#include "cube.vert"
		;
	const char* cubes_geom =
#include "cube.geom"
		;
};

#endif