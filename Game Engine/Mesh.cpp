/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Mesh.cpp
Purpose: Create VAOs of Objects
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 11/09/2018
- End Header --------------------------------------------------------*/

#include "Mesh.h"
#include <iostream>
#include "Render Manager.h"
#include <iostream>

extern RenderManager * gpRenderManager;
Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

GLuint Mesh::CreateSquare() {
	GLuint VAO, VBO, EBO;

	float vertices[] = { 	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f,  // top left 
					0.5f,  0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
					0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
					-0.5f, -0.5f, 0.0f,			  1.0f, 1.0f, 0.0f,   0.0f, 0.0f // bottom left
					  
	};
	int	size = sizeof(vertices);
	
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 2,  // first Triangle
		2, 3, 0   // second Triangle
	};
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//	gpRenderManager->EBO = EBO;
	return VAO;
}

GLuint Mesh::CreateTriangle() {
	GLuint VAO, VBO;
	float vertices[] = { -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  
					0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  
					0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  0.5f, 1.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	return VAO;
}

GLuint Mesh::CreateCircle() {
	GLuint VAO, VBO;
	float vertices[] = { 0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
			 0.433f,   0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.353f,  0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			  0.25f,  0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			   0.0f,    0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
			 -0.25f,  0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.353f,  0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.433f,   0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			  -0.5f,    0.0f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.433f,  -0.25f,  0.0f, 1.0f, 1.0f, 0.0f,
			-0.353f, -0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			 -0.25f, -0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			   0.0f,   -0.5f,  0.0f, 1.0f, 1.0f, 0.0f,
			  0.25f, -0.433f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.353f, -0.353f,  0.0f, 1.0f, 1.0f, 0.0f,
			 0.433f,  -0.25f,  0.0f, 1.0f, 1.0f, 0.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	return VAO;
}

GLuint Mesh::CreateLine() {
	GLuint VAO, VBO;
	float vertices[] = { 0.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
						0.5f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f
	};
	int	size = sizeof(vertices);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return VAO;
}