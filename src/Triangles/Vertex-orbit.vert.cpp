//shader version
#version 150 core

//projectionMatrix*modelViewMatrix
uniform mat4 modelViewProjectionMatrix;

in vec3 pos;

void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(pos.xyz,1);
}
