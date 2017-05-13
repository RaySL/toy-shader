#include "shadertoy.h"

/**
 * test.cpp
 *
 * Contains the entry point
 *
 **/

int main(int argc, char* argv[]){
  ShaderToy* st = new ShaderToy();

  st->addShader("./shaders/implicit.vert", GL_VERTEX_SHADER);
  st->addShader("./shaders/cube.glsl", GL_FRAGMENT_SHADER);

  st->liveDisplay();

  delete st;

  return 0;
}
