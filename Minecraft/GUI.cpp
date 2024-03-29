#include "gui.h"
#include <jpegio.h>
#include <iostream>
#include <algorithm>
#include <debuggl.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#define M_PI       3.14159265358979323846   // pi

const float kNear = 0.2f;
const float kFar = 600.0f;
const float kFov = 45.0f;

namespace {
	// FIXME: Implement a function that performs proper
	//        ray-cylinder intersection detection
	// TIPS: The implement is provided by the ray-tracer starter code.
}

GUI::GUI(GLFWwindow* window, int view_width, int view_height)
	:window_(window)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, KeyCallback);
	glfwSetCursorPosCallback(window_, MousePosCallback);
	glfwSetMouseButtonCallback(window_, MouseButtonCallback);
	glfwSetScrollCallback(window_, MouseScrollCallback);

	glfwGetWindowSize(window_, &window_width_, &window_height_);
	if (view_width < 0 || view_height < 0) {
		view_width_ = window_width_;
		view_height_ = window_height_;
	}
	else {
		view_width_ = view_width;
		view_height_ = view_height;
	}
	float aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ = glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	orientation_ = glm::mat3(glm::eulerAngleYXZ(axis.x, axis.y, axis.z));
	up_ = glm::column(orientation_, 1);
	look_ = glm::column(orientation_, 2);
	tangent_ = glm::column(orientation_, 0);
}

GUI::~GUI()
{
}

void GUI::keyCallback(int key, int scancode, int action, int mods)
{
#if 0
	if (action != 2)
		std::cerr << "Key: " << key << " action: " << action << std::endl;
#endif
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GL_TRUE);
		return;
	}if (key == GLFW_KEY_F && action == GLFW_PRESS && mods == GLFW_MOD_CONTROL) {
		god = !god;
		return;
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
	

	if (mods == 0 && captureWASDUPDOWN(key, action))
		return;
}

void GUI::readAndSave() {
	unsigned char *frame = (unsigned char*)malloc(3 * window_height_ * window_width_);
	glReadPixels(0, 0, window_width_, window_height_, GL_RGB, GL_UNSIGNED_BYTE, frame);
	SaveJPEG("beep.jpg", window_width_, window_height_, frame);
}

bool GUI::jump()
{
	bool toRet = jumping;
	jumping = false;
	return toRet;
}

void GUI::mousePosCallback(double mouse_x, double mouse_y)
{
	last_x_ = current_x_;
	last_y_ = current_y_;
	current_x_ = mouse_x;
	current_y_ = window_height_ - mouse_y;
	float delta_x = current_x_ - last_x_;
	float delta_y = current_y_ - last_y_;
	glm::vec3 mouse_direction = glm::normalize(glm::vec3(delta_x, delta_y, 0.0f));
	glm::vec2 mouse_start = glm::vec2(last_x_, last_y_);
	glm::vec2 mouse_end = glm::vec2(current_x_, current_y_);
	glm::uvec4 viewport = glm::uvec4(0, 0, view_width_, view_height_);


	axis = glm::vec3(-mouse_direction.x * rotation_speed_ + axis.x, -mouse_direction.y * rotation_speed_ + axis.y, 0.0f);
	axis.y = std::max(axis.y, -3.14f / 2.75f);
	axis.y = std::min(axis.y, 3.14f / 2.75f);
	orientation_ = glm::mat3(glm::eulerAngleYXZ(axis.x, axis.y, axis.z));
	up_ = glm::column(orientation_, 1);
	look_ = glm::column(orientation_, 2);
	tangent_ = glm::column(orientation_, 0);
}

void GUI::mouseButtonCallback(int button, int action, int mods)
{
	if (current_x_ <= view_width_) {
		drag_state_ = (action == GLFW_PRESS);
		current_button_ = button;
		return;
	}
}

void GUI::mouseScrollCallback(double dx, double dy)
{
	if (current_x_ < view_width_)
		return;
	scroll = dy;
}

void GUI::updateMatrices()
{
	if (keys[GLFW_KEY_SPACE]) {
		jumping = true;
	}if (keys[GLFW_KEY_W]) {
		if (fps_mode_)
			moveDir += zoom_speed_ * look_;
		else
			camera_distance_ -= zoom_speed_;
	}
	if (keys[GLFW_KEY_S]) {
		if (fps_mode_)
			moveDir -= zoom_speed_ * look_;
		else
			camera_distance_ += zoom_speed_;
	}
	if (keys[GLFW_KEY_A]) {
		if (fps_mode_)
			moveDir += pan_speed_ * tangent_;
		else
			center_ += pan_speed_ * tangent_;
	}
	if (keys[GLFW_KEY_D]) {
		if (fps_mode_)
			moveDir -= pan_speed_ * tangent_;
		else
			center_ -= pan_speed_ * tangent_;
	}
	// Compute our view, and projection matrices.
	if (fps_mode_)
		center_ = eye_ + camera_distance_ * look_;
	else
		eye_ = center_ - camera_distance_ * look_;

	view_matrix_ = glm::lookAt(eye_, center_, up_);
	light_position_ = glm::vec4(eye_, 1.0f);

	aspect_ = static_cast<float>(view_width_) / view_height_;
	projection_matrix_ =
		glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	model_matrix_ = glm::mat4(1.0f);
}

MatrixPointers GUI::getMatrixPointers() const
{
	MatrixPointers ret;
	ret.projection = &projection_matrix_;
	ret.model = &model_matrix_;
	ret.view = &view_matrix_;
	return ret;
}

bool GUI::captureWASDUPDOWN(int key, int action)
{
	bool toRet = false;
	return toRet;
}


// Delegrate to the actual GUI object.
void GUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->keyCallback(key, scancode, action, mods);
}

void GUI::MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mousePosCallback(mouse_x, mouse_y);
}

void GUI::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(button, action, mods);
}

void GUI::MouseScrollCallback(GLFWwindow* window, double dx, double dy)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseScrollCallback(dx, dy);
}
