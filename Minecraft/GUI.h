#ifndef GUI_H
#define GUI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <ctime>

struct Mesh;

/*
 * Hint: call glUniformMatrix4fv on thest pointers
 */
struct MatrixPointers {
	const glm::mat4 *projection, *model, *view;
};

class GUI {
public:
	GUI(GLFWwindow*, int view_width = -1, int view_height = -1);
	~GUI();

	void keyCallback(int key, int scancode, int action, int mods);
	void mousePosCallback(double mouse_x, double mouse_y);
	void mouseButtonCallback(int button, int action, int mods);
	void mouseScrollCallback(double dx, double dy);
	void updateMatrices();
	MatrixPointers getMatrixPointers() const;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void MouseScrollCallback(GLFWwindow* window, double dx, double dy);

	glm::vec3 getCenter() const { return center_; }
	const glm::vec3& getCamera() const { return eye_; }
	const float* getLightPositionPtr() const { return &light_position_[0]; }
	void readAndSave();
	glm::vec3 eye_ = glm::vec3(0.0f, 20.1f, camera_distance_);
	glm::vec3 moveDir = glm::vec3(0.0, 0.0, 0.0);
	bool jump();
	bool god = true;
private:
	GLFWwindow* window_;

	int window_width_, window_height_;
	int view_width_, view_height_;
	int preview_height_;
	//max key id in glfw
	bool keys[349];

	bool drag_state_ = false;
	bool fps_mode_ = true;
	bool jumping = false;
	int current_button_ = -1;
	float last_x_ = 0.0f, last_y_ = 0.0f, current_x_ = 0.0f, current_y_ = 0.0f;
	float camera_distance_ = 1.0f;
	float pan_speed_ = 0.15f;
	float rotation_speed_ = 0.06f;
	float zoom_speed_ = 0.15f;
	float aspect_;
	float scroll = 0;


	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 tangent_ = glm::cross(look_, up_);
	glm::vec3 center_ = eye_ - camera_distance_ * look_;
	glm::mat3 orientation_ = glm::mat3(tangent_, up_, look_);
	glm::vec4 light_position_;
	glm::vec3 axis = glm::vec3(3.14, 0.0,0.0);

	glm::mat4 view_matrix_ = glm::lookAt(eye_, center_, up_);
	glm::mat4 projection_matrix_;
	glm::mat4 model_matrix_ = glm::mat4(1.0f);

	bool captureWASDUPDOWN(int key, int action);

	bool play_ = false;
};

#endif
