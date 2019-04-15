#ifndef CAMERA_H
#define CAMERA_H
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
	vec4 position;
	//First person camera so we store camera rot with x,y,z rot
	vec3 axis;
private:
};

#endif