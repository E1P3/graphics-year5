#ifndef MESH_H
#define MESH_H

#include <vector>
#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "texture.h"
#include "shader.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    // bitangent
    glm::vec3 Bitangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};


class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture*> textures)
    {
        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        // now that we have all the required data, set the vertex buffers and its attribute pointers.
        setupMesh();
    }

    ~Mesh() = default;

    unsigned int getID() const {
        return ID;
    }

    void setID(unsigned int ID) {
        this->ID = ID;
    }

    void Draw(Shader* shader, bool useOwnTextures = true, bool drawTessalated = false)
    {
        if(useOwnTextures){        // bind appropriate textures
            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int heightNr = 1;

            shader->SetInteger("hasDiffuse", 0);
            shader->SetInteger("hasSpecular", 0);
            shader->SetInteger("hasNormal", 0);
            shader->SetInteger("hasHeight", 0);

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i + 1); // active proper texture unit before binding
                // retrieve texture number (the N in diffuse_textureN)
                std::string number;
                std::string name;
                TextureType type = textures[i]->getType();
                if (type == DIFFUSE){
                    number = std::to_string(diffuseNr++);
                    name = "texture_diffuse";
                    shader->SetInteger((name).c_str(), i + 1);
                    shader->SetInteger("hasDiffuse", 1);
                }
                else if (type == SPECULAR){
                    number = std::to_string(specularNr++); // transfer unsigned int to string
                    name = "texture_specular";
                    shader->SetInteger((name).c_str(), i + 1);
                    shader->SetInteger("hasSpecular", 1);
                }
                else if (type == NORMAL){
                    number = std::to_string(normalNr++); // transfer unsigned int to string
                    name = "texture_normal";
                    shader->SetInteger((name).c_str(), i + 1);
                    shader->SetInteger("hasNormal", 1);
                }
                else if (type == HEIGHT){
                    number = std::to_string(heightNr++); // transfer unsigned int to string
                    name = "texture_height";
                    shader->SetInteger((name).c_str(), i + 1);
                    shader->SetInteger("hasHeight", 1);
                }  
                glBindTexture(GL_TEXTURE_2D, textures[i]->getID());
            }
        }
        // draw mesh
        glBindVertexArray(VAO);
        if(drawTessalated){
            glPatchParameteri(GL_PATCH_VERTICES, 3);
            glDrawElements(GL_PATCHES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        }
        else{
            glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        }
        //glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // always good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

    std::vector<glm::vec3> getVertices() {
        std::vector<glm::vec3> _verticies;
        for (Vertex vertex : vertices) {
            _verticies.push_back(vertex.Position);
        }
        return _verticies;
    }

private:

    unsigned int VAO, VBO, EBO, ID;
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture*>     textures;

    void setupMesh()
    {
        // create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // set the vertex attribute pointers
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
        // ids
        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

        // weights
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
        glBindVertexArray(0);
    }
};

#endif // MESH_H
