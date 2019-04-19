#include "glSetups.h"

void frameBufferSetup(GLuint & FrameBuffer, GLuint & mainRenderTex, GLuint & depthrenderbuffer, GLenum* DrawBuffers, int width, int height)
{
	//FBO setup
	glGenFramebuffers(1, &FrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);

	//Setup tex to render to
	glGenTextures(1, &mainRenderTex);
	glBindTexture(GL_TEXTURE_2D, mainRenderTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	//DepthBuffer Setup
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, mainRenderTex));
	CHECK_GL_ERROR(glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mainRenderTex, 0));
	
	//Draw Buffer bind
	glDrawBuffers(1, DrawBuffers);
}

void glCleanup()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LESS);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void chunkSetup(int width, int height, GLuint& FrameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer);
	glViewport(0, 0, width, height);
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0, 0.5, 1.0, 1.0);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
