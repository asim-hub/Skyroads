#version 330

// TODO: get vertex attributes from each location
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec3 vertex_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 object_color;

// TODO: output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;

void main()
{
	// TODO: send output to fragment 
	frag_position	= vertex_position;
	frag_normal		= vertex_normal;
	frag_color		= object_color;

	// TODO: compute gl_Position
	gl_Position	= Projection * View * Model * vec4(vertex_position, 1.f);
}