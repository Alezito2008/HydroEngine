#include "graphics/Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertices,
           const std::vector<unsigned int>& indices,
           const std::vector<MeshTexture>& textures)
    : vertices(vertices), indices(indices), textures(textures), vb(nullptr), ib(nullptr) {
    setupMesh();
}

Mesh::~Mesh() {
    delete vb;
    delete ib;

    // NO liberar las texturas aquí, son propiedad de Model y se gestionan mediante caché
    // Las texturas se reutilizan entre meshes, por lo que Model es responsable de liberarlas
}

// Constructor de movimiento
Mesh::Mesh(Mesh&& other) noexcept
    : vertices(std::move(other.vertices)),
      indices(std::move(other.indices)),
      textures(std::move(other.textures)),
      vao(std::move(other.vao)),
      vb(other.vb),
      ib(other.ib) {
    // Transferir la propiedad de los punteros
    other.vb = nullptr;
    other.ib = nullptr;
}

// Operador de asignación de movimiento
Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        // Liberar recursos existentes (excepto texturas, que son propiedad de Model)
        delete vb;
        delete ib;

        // Mover datos
        vertices = std::move(other.vertices);
        indices = std::move(other.indices);
        textures = std::move(other.textures);
        vao = std::move(other.vao);
        vb = other.vb;
        ib = other.ib;

        // Transferir la propiedad
        other.vb = nullptr;
        other.ib = nullptr;
    }
    return *this;
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

        std::string uniformName = name + number;
        shader.setInt(uniformName, i);
        textures[i].texture->Bind(i);
    }
    renderer.Draw(vao, *ib, shader);
}

void Mesh::setupMesh() {
    vao.Bind();

    vb = new VertexBuffer(vertices.data(), vertices.size() * sizeof(Vertex));

    // Configurar atributos manualmente con el stride correcto
    vb->Bind();

    // El stride es el tamaño completo del struct Vertex
    GLsizei stride = sizeof(Vertex);

    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Position));
    glEnableVertexAttribArray(0);

    // Normal (location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Normal));
    glEnableVertexAttribArray(1);

    // TexCoords (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, TexCoords));
    glEnableVertexAttribArray(2);

    ib = new IndexBuffer(indices.data(), indices.size());

    vao.Unbind();
}
