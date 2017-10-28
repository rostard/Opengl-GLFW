//
// Created by rostard on 16.09.17.
//

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

void Shader::Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar* geometrySource) {
    std::cerr<<"Compile shader"<<std::endl;

    std::cerr<<geometrySource<<std::endl;
    std::cerr<<vertexSource<<std::endl;
    GLuint vertex, fragment, geometry;
    // vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);
    // print compile errors if any
    CheckCompileErrors(vertex, "VERTEX");

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);
    // print compile errors if any
    CheckCompileErrors(fragment, "FRAGMENT");

    if(geometrySource){
        // geometry Shader
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometrySource, NULL);
        glCompileShader(geometry);
        // print compile errors if any
        CheckCompileErrors(geometry, "GEOMETRY");
    }

    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    if (geometrySource){
        glAttachShader(this->ID, geometry);
    }
    glLinkProgram(this->ID);

    CheckCompileErrors(this->ID, "PROGRAM");
    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    if (geometrySource != nullptr){
        glDeleteShader(geometry);
    }
}

Shader& Shader::use() {
    glUseProgram(this->ID);
    return *this;
}


void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const std::string &name, glm::vec3 vector) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), vector.x, vector.y, vector.z);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::CheckCompileErrors(GLuint object, std::string type) {
    int success;
    char infoLog[1024];

    if(type != "PROGRAM"){
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::Compile-time error: " << type << std::endl << infoLog << std::endl;
        };
    } else{
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(object, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::Link-time error: " << type << std::endl << infoLog << std::endl;
        }
    }
}