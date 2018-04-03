/*
* OPEN GL DOCUMENTATION SITE: http://docs.gl/
* PUNO PUNO PUNO DOBAR SAJT ZA OPENGL TUTORIALE: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/ OVO MORAS PROVJERAVATI STALNO.
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				// set type to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// set type to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	// Error handling
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// On govori ovdje da hoce na stacku da napravi char array, nece pokazivac koji bi bio na heap-u jer ga mora kasnije pocistiti.
		// Ja se ne slazem sa njim, ali da bi mi bilo lakse pratiti video-e i ja cu napraviti kao i on.
		// char* message = new char[length];

		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " <<
			(type == GL_VERTEX_SHADER ? "vertext" : "fragment")
			<< " shader." << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	// delete intermediates
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// video 12
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Video 11
	glfwSwapInterval(1);

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error!" << std::endl;
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		// Vertex buffers and layouts in open GL
		/*float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
		};*/
		float positions[] = {
			-0.5f, -0.5f, // 0
			0.5f, -0.5f, // 1
			0.5f, 0.5f, // 2
			-0.5f, 0.5f // 3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// video 12
		unsigned int vao;
		GLCall(glGenVertexArrays(1, &vao));
		GLCall(glBindVertexArray(vao));

		VertexBuffer vb(positions, 4 * 2 * sizeof(float));

		// Vertex attributes and layouts in open GL
		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));
		GLCall(glEnableVertexAttribArray(0));

		IndexBuffer ib(indices, 6);

		// Creating shaders
		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
		unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
		glUseProgram(shader);

		// video 11 - Uniforms - sending color from code to shader
		GLCall(int location = glGetUniformLocation(shader, "u_Color"));
		ASSERT(location != -1);
		GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

		// video 12 - Vertex arrays
		// we are unbidning everything so we can see how we would do this with vertex array
		GLCall(glBindVertexArray(0));
		GLCall(glUseProgram(0));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		// video 11
		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			// Legacy (Old) OpenGl triangle drawing
			//glBegin(GL_TRIANGLES);
			//glVertex2f(-0.5f, -0.5f);
			//glVertex2f(0.0f, 0.5f);
			//glVertex2f(0.5f, -0.5f);
			//glEnd();

			// Crtanje sa nizovima vertexa
			// Za trougao bio treci parametar "3"
			// glDrawArrays(GL_TRIANGLES, 0, 6);

			// Index buffer crtanje
			// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

			// GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // uncomment to test error func


			// video 12
			// before we call drawing functions we have to bind everything
			GLCall(glUseProgram(shader));												   // bind shader
			GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));							   // set-up uniforms
			GLCall(glBindVertexArray(vao));
			/*
			 *As of video 12, 11th min, we dont have to do this anymore
				GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));								   //
				GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0)); // we bind our vertex buffer
				GLCall(glEnableVertexAttribArray(0));										   // set-up layout of that vertex buffer
			*/
			ib.Bind();

			// GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // this has been commented out cause of video 12
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			if (r > 1.0f)
				increment = -0.05f;
			else if (r < 0.0f)
				increment = 0.05f;
			r += increment;

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		// Delete shader in the end
		glDeleteProgram(shader);
	}

	glfwTerminate();
	return 0;
}
