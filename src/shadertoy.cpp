#include "shadertoy.h"

/**
 * ShaderToy.cpp
 *
 * An app to display a ShaderToy-like program on the desktop
 *
 **/


ShaderToy::ShaderToy():
	SDLGLApp("ShaderToy", 512, 512)
{}

ShaderToy::ShaderToy(const char* name, int width, int height):
	SDLGLApp(name, width, height)
{}

ShaderToy::~ShaderToy(){

}

void ShaderToy::addShader(const char* fileName, GLuint type){
	m_shaderFiles.push_back(fileName);
	m_shaderTypes.push_back(type);
}

void ShaderToy::liveDisplay(){
	// Create a program
	program = createProgram(&m_shaderFiles[0], &m_shaderTypes[0], m_shaderFiles.size());
	
	// Use the program.
	glUseProgram(program);

	// Create the VAO for the program.
	GLuint g_vao;
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);


	// Handle uniforms (I want to generalize this at some point)
	GLuint time_u = glGetUniformLocation(program, "time");

	GLuint res_u = glGetUniformLocation(program, "res");
	glUniform2f(res_u, m_width, m_height);


	// Main loop
	bool running = true;
	SDL_Event event;
	Uint32 frametime;
	const Uint32 minframetime = 1000 / 60;

	while (running) {
		frametime = SDL_GetTicks();
		glUniform1f(time_u, frametime / 1000.0);

		// Search for quit event or escape key
		while (SDL_PollEvent (&event) != 0){
			switch (event.type){
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						running = false;
					break;
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		// Set the background (not strictly necessary)
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Draw the scene
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Double buffer
		SDL_GL_SwapWindow(m_window);

		// Ensure framerate
		if (SDL_GetTicks () - frametime < minframetime)
						SDL_Delay (minframetime - (SDL_GetTicks () - frametime));
	}

	// Clean up
	glDeleteVertexArrays(1, &g_vao);
	glDeleteProgram(program);
}

GLuint ShaderToy::createProgram(const char* shaderFiles[], GLuint shaderTypes[], int shaderCount){
	// Actually create the program object
	GLuint program = glCreateProgram();

	// Create an array of shaders
	GLuint* shaders = new GLuint[shaderCount];

	// Handle each shader individually
	for (int i = 0; i < shaderCount; i++){
		const char* source = loadSource(shaderFiles[i]);
		shaders[i] = createShader(source, shaderTypes[i]);

		glAttachShader(program, shaders[i]);
	}

	glLinkProgram(program);

	// Check for linker errors
	int logLength;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

	if (logLength > 1){
		std::vector<char> programError(logLength);
		glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
		std::cout << &programError[0] << std::endl;
	}


	// Delete all the shader objects
	for (int i = 0; i < shaderCount; i++){
		glDeleteShader(shaders[i]);
	}

	return program;
}


GLuint ShaderToy::createShader(const char* source, GLuint shaderType){
	// Actually create the shader
	GLuint shader = glCreateShader(shaderType);

	int logLength;

	// Compile shader
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	// Check for shader compile errors
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 1){
		std::vector<char> shaderError(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
		std::cout << &shaderError[0] << std::endl;
	}

	return shader;
}

const char* ShaderToy::loadSource(const char* fileName){
	// Create the stream
	std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);

	// If that was successful
	if (file.is_open()){
		// Create a char* to hold the contents
		std::streampos size = file.tellg();
		char* source = new char [size];

		// Read the file
		file.seekg(0, std::ios::beg);
		file.read(source, size);
		file.close();

		// Null terminate the char*
		source[((int)size) - 1] = '\0';

		return source;
	}

	return "";
}

