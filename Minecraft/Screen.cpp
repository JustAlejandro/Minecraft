#include "Screen.h"

Screen::Screen()
{
	//Setup verts
	quad_vert.push_back(glm::vec4(-1.0f, -1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(1.0f, -1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(1.0f, 1.0f, 0.5, 1.0f));
	quad_vert.push_back(glm::vec4(-1.0f, 1.0f, 0.5, 1.0f));
	//Setup UVs
	quad_uv.push_back(glm::vec2(0.0, 0.0));
	quad_uv.push_back(glm::vec2(1.0, 0.0));
	quad_uv.push_back(glm::vec2(1.0, 1.0));
	quad_uv.push_back(glm::vec2(0.0, 1.0));
	//Setup Faces
	quad_faces.push_back(glm::uvec3(0, 1, 2));
	quad_faces.push_back(glm::uvec3(2, 3, 0));

	//Setup RenderPass Input
	input.assignIndex(quad_faces.data(), quad_faces.size(), 3);
	input.assign(0, "vertex_position", quad_vert.data(), quad_vert.size(), 4, GL_FLOAT);
	input.assign(1, "tex_coord_in", quad_uv.data(), quad_uv.size(), 2, GL_FLOAT);

	//Setup RenderPass
	render = new RenderPass(-1, input, 
		//Shaders
		{ screen_vert, nullptr, screen_frag }, 
		//Uniforms
		{}, 
		//Outputs
		{ "fragment_color" });
}

void Screen::toScreen(GLuint& mainRenderTex, int& width, int& height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
	glDepthFunc(GL_ALWAYS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex);
	render->setup();
	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, quad_faces.size() * 3, GL_UNSIGNED_INT, 0));
}
