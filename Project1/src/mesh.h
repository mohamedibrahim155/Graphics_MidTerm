#pragma once
#ifndef MESH_H
#define MESH_H


#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"



#include <string>
#include <vector>
#define MAX_BONE_INFLUENCE 4
struct Vertex
{

    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec4 vRGB;

  

};

struct Texture {
    unsigned int id;

    std::string type;
    std::string path;
};





class Mesh
{
public:

   

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    unsigned int VAO;

    Mesh( std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void meshDraw(Shader& shader);
private:
    
    unsigned int VBO, EBO;

    void setupMesh();
};
#endif
