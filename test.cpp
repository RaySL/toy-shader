#include "shadertoy.h"

int main(int argc, char* argv[]){
  const char* fname = &argv[1][0] ? &argv[1][0] : "tester.glsl";

  ShaderToy* st = new ShaderToy();

  st->addShader("./shaders/implicit.vert", GL_VERTEX_SHADER);
  st->addShader("./shaders/cube.glsl", GL_FRAGMENT_SHADER);

  st->liveDisplay();

  delete st;

  return 0;
}
