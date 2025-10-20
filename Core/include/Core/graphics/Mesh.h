#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"

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

    ~Mesh();

    // Deshabilitar copia
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    // Habilitar movimiento
    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void Draw(const Shader& shader, Renderer& renderer) const;

    std::vector<MeshTexture>  textures;

private:
    void setupMesh();

    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;

    VertexArray vao;
    VertexBuffer* vb;
    IndexBuffer* ib;
};

#endif
