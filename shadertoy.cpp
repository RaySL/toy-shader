#include "shadertoy.h"


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
  program = createProgram(&m_shaderFiles[0], &m_shaderTypes[0], m_shaderFiles.size());

  /*GLfloat points[] = { -1.0f,-1.0f, 0.0f, 1.0f,
												1.0f,-1.0f, 0.0f, 1.0f,
												1.0f, 1.0f, 0.0f, 1.0f,
											 -1.0f,-1.0f, 0.0f, 1.0f,
											 -1.0f, 1.0f, 0.0f, 1.0f,
												1.0f, 1.0f, 0.0f, 1.0f };*/
	// Retrieve the vertex location in the program.
	//GLuint g_vertexLocation = glGetAttribLocation(program, "a_vertex");

	//GLuint g_verticesVBO;
	//glGenBuffers(1, &g_verticesVBO);
	//glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);

	// Transfer the vertices from CPU to GPU.
	//glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(GLfloat), (GLfloat*) points, GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Use the program.
	glUseProgram(program);

	// Create the VAO for the program.
	GLuint g_vao;
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	// Bind the only used VBO in this example.
	//glBindBuffer(GL_ARRAY_BUFFER, g_verticesVBO);
	//glVertexAttribPointer(g_vertexLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(g_vertexLocation);

	GLuint time_u = glGetUniformLocation(program, "time");

  GLuint res_u = glGetUniformLocation(program, "res");
  glUniform2f(res_u, m_width, m_height);

	bool running = true;
	SDL_Event event;
	Uint32 frametime;
	const Uint32 minframetime = 1000 / 60;

	while (running) {
		frametime = SDL_GetTicks();
		glUniform1f(time_u, frametime / 1000.0);

		while (SDL_PollEvent (&event) != 0)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_ESCAPE)
						running = false;
					break;
        case SDL_QUIT:
          running = false;
          break;
			}
		}

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		SDL_GL_SwapWindow(m_window);

		if (SDL_GetTicks () - frametime < minframetime)
      SDL_Delay (minframetime - (SDL_GetTicks () - frametime));

	}

  //glDeleteBuffers(1, &g_verticesVBO);
  glDeleteVertexArrays(1, &g_vao);
  glDeleteProgram(program);
}

GLuint ShaderToy::createProgram(const char* shaderFiles[], GLuint shaderTypes[], int shaderCount){
  GLuint program = glCreateProgram();

  GLuint* shaders = new GLuint[shaderCount];

  for (int i = 0; i < shaderCount; i++){
    const char* source = loadSource(shaderFiles[i]);
    shaders[i] = createShader(source, shaderTypes[i]);

    //std::cout << shaderCount << "Source: " << source << "\n";

    glAttachShader(program, shaders[i]);
  }

  glLinkProgram(program);

  int logLength;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

  if (logLength > 1){
    std::vector<char> programError(logLength);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
  }

  for (int i = 0; i < shaderCount; i++){
    glDeleteShader(shaders[i]);
  }

  return program;
}

/*
GLuint ShaderToy::createProgram(const char* vertFile, const char* fragFile){
  std::cout << "Creating vertex shader\n";
  const char* vertSource = loadSource(vertFile);
  GLuint vert = createShader(vertSource, GL_VERTEX_SHADER);

  std::cout << "Creating fragment shader\n";
  const char* fragSource = loadSource(fragFile);
  GLuint frag = createShader(fragSource, GL_FRAGMENT_SHADER);

  std::cout << "Linking shader program\n";
  GLuint program = createProgram(vert, frag);

  glDeleteShader(vert);
  glDeleteShader(frag);

  return program;
}
GLuint ShaderToy::createProgram(GLuint shader1, GLuint shader2){
  GLuint program = glCreateProgram();
  glAttachShader(program, shader1);
  glAttachShader(program, shader2);
  glLinkProgram(program);

  int logLength;
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1){
    std::vector<char> programError(logLength);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
  }

  glDeleteShader(shader1);
  glDeleteShader(shader2);

  return program;
}
*/

GLuint ShaderToy::createShader(const char* source, GLuint shaderType){
  GLuint shader = glCreateShader(shaderType);

  int logLength;

  // Compile shader
  //std::cout << "Source:\n" << source << std::endl;
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  // Check shader
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
  if (logLength > 1){
    std::vector<char> shaderError(logLength);
    glGetShaderInfoLog(shader, logLength, NULL, &shaderError[0]);
    std::cout << &shaderError[0] << std::endl;
  }

  return shader;
}

const char* ShaderToy::loadSource(const char* fileName){
  std::ifstream file (fileName, std::ios::in|std::ios::binary|std::ios::ate);

  if (file.is_open()){
    std::streampos size = file.tellg();
    char* source = new char [size];

    file.seekg(0, std::ios::beg);
    file.read(source, size);
    file.close();

    source[((int)size) - 1] = '\0';

    return source;
  }

  return "";
}
