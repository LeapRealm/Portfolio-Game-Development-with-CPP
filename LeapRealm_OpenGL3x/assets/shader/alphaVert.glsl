#version 150

uniform mat4 mProject, mModelview;

in vec4 position;
in vec4 color;
in vec2 st;

out vec4 colorV;
out vec2 stV;

void main()
{
	gl_Position = mProject * mModelview * position;
	colorV = color;
	stV = st;
}
