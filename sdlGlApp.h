#ifndef _SDLGLAPP_H_
#define _SDLGLAPP_H_

#include <iostream>

#include <SDL2/SDL.h>

#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/glxext.h>

//#define GL3_PROTOTYPES 1
//#include <GL3/gl3.h>

class SDLGLApp {
public:
  SDLGLApp(const char* name, int width, int height);
  ~SDLGLApp();

  SDL_Window* createWindow(const char* name, int width, int height);
  SDL_GLContext createGLContext(int major, int minor);

protected:
  int m_width;
  int m_height;

  SDL_Window* m_window;
  SDL_GLContext m_glCtx;
};


#endif
