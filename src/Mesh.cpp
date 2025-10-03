#include "include/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<MeshTexture>& textures)
    : vertices(vertices), indices(indices), textures(textures), vb(nullptr), ib(nullptr, 0) {
    setupMesh();
}

void Mesh::Draw(const Shader& shader, Renderer& renderer) const {
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;

    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string number;
        std::string name = textures[i].type;

        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);
        else if (name == "texture_normal")
            number = std::to_string(normalNr++);
        else if (name == "texture_height")
            number = std::to_string(heightNr++);

        shader.setInt(name + number, i);
        textures[i].texture->Bind(i);
    }

    renderer.Draw(vao, ib, shader);
}

void Mesh::setupMesh() {
    vao.Bind();

    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));
    VertexBufferLayout layout;
    layout.Push<float>(3); // Position
    layout.Push<float>(3); // Normal
    layout.Push<float>(2); // TexCoords
    layout.Push<float>(3); // Tangent
    layout.Push<float>(3); // Bitangent
    layout.Push<int>(4);   // Bone IDs
    layout.Push<float>(4); // Weights

    vao.AddBuffer(*vb, layout);
    ib = IndexBuffer(indices.data(), indices.size());

    vao.Unbind();
}
