#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


#include "include/Mesh.h"
#include "include/Shader.h"
#include "include/Renderer.h"
#include "Texture.h"

#include <string>
#include <vector>
#include <map>

struct MeshTexture;

class Model 
{
public:
    Model(const std::string& path, bool gamma = false);
    ~Model();

    void Draw(Shader& shader, Renderer& renderer) const;

private:
    void loadModel(const std::string& path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);

private:
    std::vector<Mesh> meshes;
    std::string directory;
    bool gammaCorrection;
    std::map<std::string, Texture*> textureCache;  // Caché de texturas para reutilizarlas
};