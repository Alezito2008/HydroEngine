#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "include/Shader.h"
#include "include/Texture.h"
#include "include/VertexArray.h"
#include "include/VertexBuffer.h"
#include "include/IndexBuffer.h"
#include "include/Renderer.h"

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
    int   m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];
};

struct MeshTexture {
    Texture* texture;   // puntero a tu clase Texture
    std::string type;   // "texture_diffuse", etc.
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices,
         const std::vector<unsigned int>& indices,
         const std::vector<MeshTexture>& textures);

    void Draw(const Shader& shader, Renderer& renderer) const;

private:
    void setupMesh();

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture>  textures;

    VertexArray vao;
    VertexBuffer* vb;
    IndexBuffer ib;
};

#endif
