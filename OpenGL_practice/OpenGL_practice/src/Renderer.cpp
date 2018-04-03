#include "Renderer.h"
#include <iostream>


void GLClearError()
{
	// while (!glGetError()); // Can be done like this as well
	while (glGetError() != GL_NO_ERROR)
	{
		// std::cout << "radi ovo" << std::endl;
	}
}

bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGl error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}