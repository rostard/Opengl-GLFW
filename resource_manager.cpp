#include "resource_manager.h"
#include "stb_image.h"


std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader& ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    std::cerr<<vShaderFile<<std::endl;
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name) {
    return Shaders[name];
}

Texture2D& ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name) {
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name) {
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (Properly) delete all shaders
    for (auto it : Shaders)
        glDeleteProgram(it.second.ID);
    // (Properly) delete all textures
    for (auto it : Textures)
        glDeleteTextures(1, &it.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile) {
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try {
        std::ifstream vertexShaderFile(vShaderFile);
        std::string test;
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(gShaderFile != nullptr){
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
        const GLchar* vShaderCode = vertexCode.c_str();
        const GLchar* fShaderCode = fragmentCode.c_str();
        const GLchar* gShaderCode = geometryCode.c_str();
        Shader shader;

        shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
        return shader;
    }
    catch (std::exception e){
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha) {
    Texture2D texture;
    if(alpha){
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    int width, height;

    unsigned char* image = stbi_load(file, &width, &height, nullptr, alpha ? 4 : 3);
    if(!image){
        std::cerr<<"error loading image path: "<<file<<std::endl;
    }

    texture.Generate(width, height, image);
    stbi_image_free(image);

    return texture;

}


