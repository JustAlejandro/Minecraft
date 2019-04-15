#include "Cube.h"

Cubes::Cubes(const mat4* viewIn, const mat4* projIn, const vec4* lightIn)
{
	proj = mat4(*projIn);
	view = mat4(*projIn);
	light = vec4(*lightIn);
	//Cube Vertices
	cube_vert.push_back(glm::vec4(-1, -1, -1, 1.0));//BL 0
	cube_vert.push_back(glm::vec4(-1, 1, -1, 1.0));//TL 1
	cube_vert.push_back(glm::vec4(1, 1, -1, 1.0));//TL 2
	cube_vert.push_back(glm::vec4(1, -1, -1, 1.0));//TR 3
	cube_vert.push_back(glm::vec4(-1, -1, 1, 1.0));//BL 4
	cube_vert.push_back(glm::vec4(-1, 1, 1, 1.0));//TL 5
	cube_vert.push_back(glm::vec4(1, 1, 1, 1.0));//TL 6 
	cube_vert.push_back(glm::vec4(1, -1, 1, 1.0));//TR 7
	//Front?
	cube_faces.push_back(glm::uvec3(2, 1, 0));
	cube_faces.push_back(glm::uvec3(3, 2, 0));
	//Back
	cube_faces.push_back(glm::uvec3(5, 6, 7));
	cube_faces.push_back(glm::uvec3(4, 5, 7));
	//Left
	cube_faces.push_back(glm::uvec3(0, 1, 5));
	cube_faces.push_back(glm::uvec3(5, 4, 0));
	//Right?
	cube_faces.push_back(glm::uvec3(7, 6, 2));
	cube_faces.push_back(glm::uvec3(2, 3, 7));
	//Top?
	cube_faces.push_back(glm::uvec3(1, 2, 6));
	cube_faces.push_back(glm::uvec3(6, 5, 1));
	//Bot?
	cube_faces.push_back(glm::uvec3(4, 3, 0));
	cube_faces.push_back(glm::uvec3(3, 4, 7));

	//Setup RenderPass Input
	input.assignIndex(cube_faces.data(), cube_faces.size(), 3);
	input.assign(0, "vertex_position", cube_vert.data(), cube_vert.size(), 4, GL_FLOAT);
	input.assign(1, "tex_coord_in", cube_uv.data(), cube_uv.size(), 2, GL_FLOAT);

	std::function<std::vector<vec4>()> loc_data = [this]() { return cube_location; };
	auto loc_uni = make_uniform("trans", loc_data);

	std::function<mat4()> proj_data = [this]() { return proj; };
	auto proj_uni = make_uniform("projection", proj_data);

	std::function<mat4()> view_data = [this]() { return view; };
	auto view_uni = make_uniform("view", view_data);

	std::function<vec4()> light_data = [this]() { return light; };
	auto light_uni = make_uniform("light_direction", light_data);

	//Setup RenderPass
	render = new RenderPass(-1, input,
		//Shaders
		{ cubes_vert, cubes_geom, cubes_frag },
		//Uniforms
		{ loc_uni, proj_uni, view_uni, light_uni },
		//Outputs
		{ "fragment_color" });
}

void Cubes::add(vec4 loc)
{
	count++;
	cube_location.push_back(loc);
}

void Cubes::toScreen(GLuint & FrameBuffer, MatrixPointers mat, vec4 l, int & width, int & height)
{
	proj = mat4(*mat.projection);
	view = mat4(*mat.view);
	light = vec4(l);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	render->setup();
	CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, cube_faces.size() * 3, GL_UNSIGNED_INT, 0, count));	
}