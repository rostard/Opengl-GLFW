//
// Created by rostard on 27.09.17.
//

#ifndef OPENGL_GLFW_MODEL_H
#define OPENGL_GLFW_MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.h"

class Model{
public:

    Model(const char* path)
    {
        loadModel(path);
    }
    void Draw(Shader shader);
    vector<Mesh> meshes;
private:
    vector<Texture> textures_loaded;

    string directory;

    void loadModel(string path);
    void processNode(aiNode *node, const aiScene *scene);
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);
    vector<Texture> loadMaterialsTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif //OPENGL_GLFW_MODEL_H
