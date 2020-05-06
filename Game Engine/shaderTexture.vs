#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform vec2 textureOffSet;
uniform vec2 mulTexture;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = projection * transform * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(  ( aTexCoord.x * mulTexture.x ) + textureOffSet.x , 
					  ( aTexCoord.y * mulTexture.y ) + textureOffSet.y);
}