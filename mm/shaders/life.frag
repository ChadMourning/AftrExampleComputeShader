#version 430 core
out vec4 FragColor;
	
in vec2 TexCoords;
	
uniform sampler2D TexUnit0;
	
void main()
{             
    vec3 texCol = texture(TexUnit0, TexCoords).rgb;      
    FragColor = vec4(texCol, 1.0);
}