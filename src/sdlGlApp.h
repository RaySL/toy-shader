#ifndef _SDLGLAPP_H_
#define _SDLGLAPP_H_

#include <iostream>

#include <SDL2/SDL.h>


// This is platform dependant, you may need to change it
#define GL3_PROTOTYPES 1
#include <GLES3/gl3.h>


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
