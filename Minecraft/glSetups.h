#ifndef GLSETUPS_H
#define GLSETUPS_H

#include <debuggl.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "render_pass.h"
/* This file is used to keep a lot of the bloat out of our main render loop
*/
void frameBufferSetup(GLuint& FrameBuffer, GLuint& mainRenderTex, GLuint& depthrenderbuffer, GLenum* DrawBuffers, int width, int height);

void glCleanup();




#endif