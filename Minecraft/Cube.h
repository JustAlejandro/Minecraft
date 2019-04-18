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
	Cubes(MatrixPointers* mat, vec4* lightIn);
	~Cubes();
	void add(vec4 loc);
	void toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height);

	static std::vector<vec4> cube_vert;
	static std::vector<vec2> cube_uv;
	static std::vector<uvec3> cube_faces;

	static std::vector<vec3> *cube_location_static;
	void setLocation();
	std::vector<vec3> cube_location;
	int count = 0;
	static void setup(MatrixPointers* mat, vec4* lightIn);
	static mat4 view;
	static mat4 proj;
	static vec4 light;
private:
	static RenderDataInput* input;
	static RenderPass* render;
	
	
};

#endif