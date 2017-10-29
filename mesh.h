//
// Created by rostard on 27.09.17.
//

#ifndef OPENGL_GLFW_MESH_H
#define OPENGL_GLFW_MESH_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <assimp/types.h>
#include "shader.h"

using namespace std;

struct Vertex{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture{
    unsigned int id;
    string type;
    aiString path;
};

class Mesh{
public:
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;

    Mesh(vector<Vertex> &vertices, vector<unsigned int> &indices, vector<Texture> &textures);

    void Draw(Shader shader);
    unsigned int VAO;
private:
    unsigned int  VBO, EBO;

    void setupMesh();
};


#endif //OPENGL_GLFW_MESH_H
