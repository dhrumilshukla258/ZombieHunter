#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <string>
#include <unordered_map>
#include <GL/glew.h>

class Mesh;
class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	GLuint LoadMesh(std::string);
	GLuint LoadTexture(std::string);
private:
	std::unordered_map<std::string, GLuint> mMeshes;
	std::unordered_map<std::string, GLuint> mTextures;
};
#endif