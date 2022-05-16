#version 150

uniform mat4 mProject, mModelview;

in vec4 position;

void main()
{
	gl_Position = mProject * mModelview * position;
}
