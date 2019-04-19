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

	vec4 light = vec4(0.0f, 1000.0f, 0.0f, 1.0f);
	vec3 playerPos = gui.getCenter();
	gui.eye_ = vec4(10.0, 30.0, -10.0, 1.0);
	World world(mats, &light);

	//Setup the quadPassThrough

	
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		playerPos = gui.getCenter();
		bool jump = gui.jump();
		vec3 mod = world.checkPlayer(gui.eye_, gui.moveDir, jump, gui.god);
		gui.eye_ = gui.eye_ + gui.moveDir * mod;
		gui.moveDir = vec3(0.0, 0.0, 0.0);

		gui.updateMatrices();
		mats = &gui.getMatrixPointers();
		chunkSetup(window_width, window_height, FrameBuffer);
		world.worldUpdate(playerPos, mats, &light);
		world.toScreen(FrameBuffer, *mats, light, window_width, window_height);
		screen.toScreen(mainRenderTex, window_width, window_height);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

	exit(EXIT_SUCCESS);
}
