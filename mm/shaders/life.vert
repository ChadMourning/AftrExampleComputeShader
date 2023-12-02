#version 430 core
layout ( location = 0 ) in vec3 VertexPosition;
layout ( location = 2 ) in vec2 VertexTexCoord;

uniform mat4 MVPMat;
	
out vec2 TexCoords;
	
void main()
{
    TexCoords = VertexTexCoord;
    gl_Position = MVPMat * vec4(VertexPosition, 1.0);
}