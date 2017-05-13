#include "sdlGlApp.h"

/**
 * sdlGlApp.cpp
 *
 * A class to handle the creation of an OpenGL context in SDL
 *
 **/

SDLGLApp::SDLGLApp(const char* name, int width, int height){
	SDL_Init(SDL_INIT_EVERYTHING);

	m_width = width;
	m_height = height;

	m_window = createWindow(name, width, height);
	m_glCtx = createGLContext(3, 2);
}

SDLGLApp::~SDLGLApp(){
	SDL_GL_DeleteContext(m_glCtx);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

SDL_Window* SDLGLApp::createWindow(const char* name, int width, int height){
	// Create the window	
	SDL_Window* window = SDL_CreateWindow(
		name,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		width,
		height,
		SDL_WINDOW_OPENGL
	);

	// Handle failure
	if (window == NULL){
		std::cerr << "Could not create window: " << SDL_GetError() << std::endl;
	}

	return window;
}

SDL_GLContext SDLGLApp::createGLContext(int major, int minor){
	if (m_window == NULL){
		std::cerr << "Cannot create gl context without window." << std::endl;
	}

	// Create the context of the specified version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, major);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minor);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GLContext glContext = SDL_GL_CreateContext(m_window);

	// Handle Failure
	if (glContext == NULL){
		std::cerr << "Could not create a valid GL context: " << SDL_GetError() << std::endl;
	}

	return glContext;
}
