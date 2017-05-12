#version 150

const vec2 vertices[] = vec2[](vec2(+1., -1.), vec2(-1., -1.), vec2(+1., +1.),
															 vec2(-1., +1.), vec2(+1., +1.), vec2(-1., -1.));
															 
void main(void){
	gl_Position = vec4(vertices[gl_VertexID], 0., 1.);
}
