//
// Created by rostard on 16.10.17.
//

#ifndef OPENGL_GLFW_FONT_H
#define OPENGL_GLFW_FONT_H

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <map>
#include <freetype/freetype.h>
#include <iostream>


#include "shader.h"
#include "resource_manager.h"


struct Character{
    GLuint TextureID;
    glm::ivec2 Size;
    glm::ivec2 Bearing;
    GLuint Advance;
};

class Font{
public:
    Font(){
        shader = &ResourceManager::LoadShader("shaders/font.vs.glsl", "shaders/font.fs.glsl", nullptr, "font");
        InitBuffers();
    }
    Font(const char* filename){
        shader = &ResourceManager::LoadShader("shaders/font.vs.glsl", "shaders/font.fs.glsl", nullptr, "font");
        LoadFont(filename);
        InitBuffers();
    }
    ~Font(){
    }
    void LoadShader(const char* vertexShader, const char* fragmentShader){
        if(shader != NULL)delete(shader);
        shader = &ResourceManager::LoadShader(vertexShader, fragmentShader, nullptr, "font");
    }
    void LoadFont(const char* filename);
    void InitBuffers();
    void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

private:
    GLuint VBO, VAO;
    Shader* shader;
    std::map<GLchar, Character> Characters;
    FT_Face face;
};

void Font::LoadFont(const char* filename){

    FT_Library ft;

    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    if (FT_New_Face(ft, filename, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 48);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for(int c = 0; c<128; c++){
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)){
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" <<std::endl;
            continue;
        }



        GLuint Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
                Texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void Font::InitBuffers() {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*6 * 4 , NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Font::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color) {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader->use();
    shader->setVec3("color", color);
    shader->setInt("text", 0);
    shader->setMat4("projection", projection);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    for(auto c = text.begin(); c!= text.end(); c++){


        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
        };

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_DEPTH_TEST);
}

#endif //OPENGL_GLFW_FONT_H
