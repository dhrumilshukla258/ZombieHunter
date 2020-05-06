/* Start Header -------------------------------------------------------
Copyright (C) 20xx DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
File Name: Resource Manager.cpp
Purpose: Use Resource Manager, implement Hash Map to store textureIds and 
		 VAOs.
Language: C++
Platform: Visual Studio 2017 - Visual C++ 14.1, Windows 10 Home
Project: CS529_dhrumilshukla_GameEngine1.0
Author: Dhrumil Shukla, dhrumil.shukla, 60001718
Creation date: 09/15/2018
- End Header --------------------------------------------------------*/

#include "Resource Manager.h"
#include "Render Manager.h"
#include "Mesh.h"
#include <string>
#include "stb_image.h"
#include <iostream>
//extern RenderManager * gpRenderManager;
ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

GLuint ResourceManager::LoadMesh( std::string shapeFile) {
	//Check Hash map
	GLuint VAO = mMeshes[shapeFile];
	
	//Already Loaded?
	if (VAO) {
		return VAO;
	}

	//Load it
	Mesh m;
	if (shapeFile == "Square" ) {
		VAO = m.CreateSquare();
	}
	else if (shapeFile == "Triangle" ) {
		VAO = m.CreateTriangle();
	}
	else if (shapeFile == "Circle") {
		VAO = m.CreateCircle();
	}
	else if (shapeFile == "Line") {
		VAO = m.CreateLine();
	}
	//Load Successfull? Add to hash map
	if (VAO) {
		mMeshes[shapeFile] = VAO;
	}

	//return to user
	return VAO;
}

GLuint ResourceManager::LoadTexture(std::string textureFile) {
	//Check Hash Map
	GLuint textureId = mTextures[textureFile];

	if (textureId) {
		return textureId;
	}

	//Load it
	{
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
		// set the texture wrapping parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// set texture filtering parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// load image, create texture and generate mipmaps
		int width, height, nrChannels;
		// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
		stbi_set_flip_vertically_on_load(true);
		unsigned char *data = stbi_load(textureFile.c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			if ( nrChannels == 4 )
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			else
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);
	}
	
	//Load Successfull? Add to hash map
	if (textureId) {
		mTextures[textureFile] = textureId;
	}
	
	//return to user
	return textureId;
}