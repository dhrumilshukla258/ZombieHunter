#pragma once
#include <GL/glew.h>
#include <string>

class Mesh
{
public:
	Mesh();
	~Mesh();
	GLuint CreateTriangle();
	GLuint CreateSquare();
	GLuint CreateCircle();
	GLuint CreateLine();
};