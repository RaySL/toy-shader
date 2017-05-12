//const vec2 quadVertices[4] = vec2[4]( vec2(-1.0, -1.0), vec2(1.0, -1.0), vec2(-1.0, 1.0), vec2(1.0, 1.0) );
#version 150

in vec4 a_vertex;

void main(void)
{
	gl_Position = a_vertex;
}
