#pragma once

#include <map>
#include <glm/glm.hpp>
#include <GL/glew.h>
#include "Shader.h"
#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
	unsigned int TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	unsigned int Advance;    // Horizontal offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();
	void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

	int shaderId;
	std::map<char, Character> Characters;
	GLuint VAO, VBO;
};