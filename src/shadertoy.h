#ifndef _SHADERTOY_H_
#define _SHADERTOY_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include <stdio.h>

#include "sdlGlApp.h"


class ShaderToy : public SDLGLApp {
public:
  ShaderToy();
  ShaderToy(const char* name, int width, int height);

  ~ShaderToy();

  void addShader(const char* file, GLuint type);
  void liveDisplay();
  
  // TODO:
  //void renderToImage(int width, int height);

  GLuint createProgram(const char* shaderFiles[], GLuint shaderTypes[], int shaderCount);
  GLuint createShader(const char* source, GLuint shaderType);
  const char* loadSource(const char* fileName);


private:
  std::vector<const char*> m_shaderFiles;
  std::vector<GLuint> m_shaderTypes;

  GLuint program;
  
  // TODO:
  //GLuint[] uniforms;
};


#endif
