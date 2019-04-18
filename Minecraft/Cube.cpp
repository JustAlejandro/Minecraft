#include "Cube.h"

const char* cubes_frag =
#include "cube.frag"
;
const char* cubes_vert =
#include "cube.vert"
;
const char* cubes_geom =
#include "cube.geom"
;

mat4 Cubes::view = mat4(1.0);
mat4 Cubes::proj = mat4(1.0);

std::vector<vec4> Cubes::cube_vert;
std::vector<vec2> Cubes::cube_uv;
std::vector<uvec3> Cubes::cube_faces;

std::vector<vec3>* Cubes::cube_location_static = NULL;
vec4 Cubes::light(0.0);

RenderDataInput* Cubes::input = NULL;
RenderPass* Cubes::render = NULL;

Cubes::Cubes(MatrixPointers* mat, vec4* lightIn)
{
	
}

Cubes::~Cubes()
{
	delete input;
	delete render;
}

void Cubes::add(vec4 loc)
{
	count++;
	cube_location.push_back(vec3(loc));
}

void Cubes::toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height)
{
	proj = mat4(*mat.projection);
	view = mat4(*mat.view);
	light = vec4(l);
	render->setup();
	CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, cube_faces.size() * 3, GL_UNSIGNED_INT, 0, cube_location_static->size()));	
}

void Cubes::setLocation()
{
	cube_location_static = &cube_location;
}


void Cubes::setup(MatrixPointers* mat, vec4* lightIn)
{
	cube_vert.push_back(glm::vec4(0, 0, 0, 1.0));//BL 0
	cube_vert.push_back(glm::vec4(0, 1, 0, 1.0));//TL 1
	cube_vert.push_back(glm::vec4(1, 1, 0, 1.0));//TL 2
	cube_vert.push_back(glm::vec4(1, 0, 0, 1.0));//TR 3
	cube_vert.push_back(glm::vec4(0, 0, 1, 1.0));//BL 4
	cube_vert.push_back(glm::vec4(0, 1, 1, 1.0));//TL 5
	cube_vert.push_back(glm::vec4(1, 1, 1, 1.0));//TL 6 
	cube_vert.push_back(glm::vec4(1, 0, 1, 1.0));//TR 7
	//Front?
	cube_faces.push_back(glm::uvec3(2, 1, 0));
	cube_faces.push_back(glm::uvec3(3, 2, 0));
	//Back
	cube_faces.push_back(glm::uvec3(7, 6, 5));
	cube_faces.push_back(glm::uvec3(7, 5, 4));
	//Left
	cube_faces.push_back(glm::uvec3(0, 1, 5));
	cube_faces.push_back(glm::uvec3(5, 4, 0));
	//Right?
	cube_faces.push_back(glm::uvec3(2, 6, 7));
	cube_faces.push_back(glm::uvec3(7, 3, 2));
	//Top?
	cube_faces.push_back(glm::uvec3(1, 2, 6));
	cube_faces.push_back(glm::uvec3(6, 5, 1));
	//Bot?
	cube_faces.push_back(glm::uvec3(4, 3, 0));
	cube_faces.push_back(glm::uvec3(3, 4, 7));

	//Setup RenderPass Input
	input = new RenderDataInput();
	input->assignIndex(cube_faces.data(), cube_faces.size(), 3);
	input->assign(0, "vertex_position", cube_vert.data(), cube_vert.size(), 4, GL_FLOAT);
	input->assign(1, "tex_coord_in", cube_uv.data(), cube_uv.size(), 2, GL_FLOAT);

	proj = mat4(*mat->projection);
	view = mat4(*mat->view);
	light = vec4(*lightIn);

	std::function<std::vector<vec3>()> loc_data = []() { return *Cubes::cube_location_static; };
	auto loc_uni = make_uniform("trans", loc_data);

	std::function<mat4()> proj_data = []() { return Cubes::proj; };
	auto proj_uni = make_uniform("projection", proj_data);

	std::function<mat4()> view_data = []() { return Cubes::view; };
	auto view_uni = make_uniform("view", view_data);

	std::function<vec4()> light_data = []() { return Cubes::light; };
	auto light_uni = make_uniform("light_position", light_data);

	//Setup RenderPass
	render = new RenderPass(-1, *input,
		//Shaders
		{ cubes_vert, cubes_geom, cubes_frag },
		//Uniforms
		{ loc_uni, proj_uni, view_uni, light_uni },
		//Outputs
		{ "fragment_color" });
}
