//
// Created by rostard on 16.09.17.
//

#ifndef TESTPROJECT_SHADER_H
#define TESTPROJECT_SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

class Shader{
public:
    // the program ID
    unsigned int ID;

    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec3(const std::string &name, glm::vec3 vector) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat4(const std::string &name, glm::mat4 matrix) const;
};

#endif //TESTPROJECT_SHADER_H
