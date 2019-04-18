#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_pass.h"
#include "glSetups.h"
#include "geometry.h"
#include "Screen.h"
#include "GUI.h"
#include "Chunk.h"
#include "World.h"

#include <memory>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <math.h>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>
#include "main.h"
using namespace glm;

int window_width = 1280;
int window_height = 720;
const std::string window_title = "Minecraft";

// FIXME: Add more shaders here.

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

bool checkExist(std::vector<Chunk*> c, vec3 pos) {
	int j;
	for (unsigned int i = 0; i < c.size(); i++)
	{
		vec3 focus = c[i]->worldPos;
		if (focus.x < pos.x + 1.0 && focus.x > pos.x - 1.0)
			if (focus.z < pos.z + 1.0 && focus.z > pos.x - 1.0)
				return true;
	}
	return false;
}

GLFWwindow* init_glefw()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Disable resizing, for simplicity
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
	CHECK_SUCCESS(ret != nullptr);
	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}

int main(int argc, char* argv[])
{
	srand(100);
	GLFWwindow *window = init_glefw();
	GUI gui(window, window_width, window_height);

	gui.updateMatrices();
	MatrixPointers* mats = &gui.getMatrixPointers();

	//PLAN: render each view one at a time to the same texture, displaying each as we go

	glfwGetFramebufferSize(window, &window_width, &window_height);

	//FrameBuffer setup
	GLuint FrameBuffer, mainRenderTex, depthrenderbuffer;
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	frameBufferSetup(FrameBuffer, mainRenderTex, depthrenderbuffer, DrawBuffers, window_width, window_height);
	std::vector<vec4> quad_vert;
	std::vector<vec2> quad_UV;
	std::vector<vec3> quad_faces;
	//Generates our verts, uv's, and faces for the quad to render to screen
	Screen screen;

	vec4 light = vec4(0.0f, 10.0f, 0.0f, 1.0f);


	Chunk chunk(mats, &light, vec4(-10.0, 0.0, -25.0, 1.0), 20, 0);
	Chunk chunk2(mats, &light, vec4(-10.0, -20.0, -75.0, 1.0), 20, 0);
	vec3 playerPos = gui.getCenter();
	World world(mats, &light);
	std::vector<Chunk*> chunks;

	//Setup the quadPassThrough

	
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		gui.updateMatrices();
		mats = &gui.getMatrixPointers();
		playerPos = gui.getCenter();
		vec3 roundedDown = vec3(float((int)playerPos.x / 10) * 10.0f, 0, float((int)playerPos.z / 10) * 10.0f);
		for (int i = -1; i < 2; i++)
		{
			for (int j = -1; j < 2; j++)
			{
				if (!checkExist(chunks, vec3(roundedDown.x + i * seed_height, 0.0, roundedDown.z + j * seed_width))) {
					chunks.push_back(new Chunk(mats, &light, vec4(roundedDown.x + i * seed_height, 0.0, roundedDown.z + j * seed_width, 1.0), 20, 0));
				}
			}

		}
		chunkSetup(window_width, window_height, FrameBuffer);
		//TODO: sort all the chunks by distance from player and render in order
		for (unsigned int i = 0; i < chunks.size(); i++)
		{
			chunks[i]->toScreen(FrameBuffer, *mats, light, window_width, window_height);
		}
		//chunk.toScreen(FrameBuffer, *mats, light, window_width, window_height);
		//chunk2.toScreen(FrameBuffer, *mats, light, window_width, window_height);
		screen.toScreen(mainRenderTex, window_width, window_height);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
