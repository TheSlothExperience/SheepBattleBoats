#define GL_GLEXT_PROTOTYPES

#include "object3d.h"
#include "vector"

#include <assert.h>

Object3D::MeshEntry::MeshEntry()
{
    vb = INVALID_VALUE;
    ib = INVALID_VALUE;
    numIndices  = 0;
    materialIndex = INVALID_MATERIAL;
};

Object3D::MeshEntry::~MeshEntry()
{
    if (vb != INVALID_VALUE)
    {
        glDeleteBuffers(1, &vb);
    }
    if (ib != INVALID_VALUE)
    {
        glDeleteBuffers(1, &ib);
    }
}

bool Object3D::MeshEntry::init(const std::vector<Vertex>& vertices,
                          const std::vector<unsigned int>& indices)
{
    numIndices = indices.size();

    glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &ib);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);
}

Object3D::Object3D(){


}

bool Object3D::loadMesh(const std::string& filename){

    //Release ma vaaderr
    //clear();

    bool loaded = false;

    Assimp::Importer importer;

    const aiScene* pScene = importer.ReadFile(filename.c_str(),aiProcess_Triangulate |
                                              aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
    if(pScene){

        loaded = initFromScene(pScene,filename);
    }else{
        printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
    }

    return loaded;
}


//First init all the meshes for the scene
bool Object3D::initFromScene(const aiScene *pScene, const std::string &filename){

    entries.resize(pScene->mNumMeshes);
    textures.resize(pScene->mNumMaterials);

    //Init the meshes in the scene
    for(unsigned int i = 0; i<entries.size();i++){
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i,paiMesh);
    }
    //after init go further to materials
    return initMaterials(pScene, filename);
}

bool Object3D::initMesh(unsigned int index, const aiMesh *paiMesh){

    entries[index].materialIndex = paiMesh->mMaterialIndex;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

        Vertex v(QVector3D(pPos->x, pPos->y, pPos->z),
                 QVector2D(pTexCoord->x, pTexCoord->y),
                 QVector3D(pNormal->x, pNormal->y, pNormal->z));

        vertices.push_back(v);
    }

    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        indices.push_back(Face.mIndices[0]);
        indices.push_back(Face.mIndices[1]);
        indices.push_back(Face.mIndices[2]);
    }

    entries[index].init(vertices, indices);

}

bool Object3D::initMaterials(const aiScene *pScene, const std::string &filename){

    // Extract the directory part from the file name
    std::string::size_type slashIndex = filename.find_last_of("/");
    std::string dir;

    if (slashIndex == std::string::npos) {
        dir = ".";
    }
    else if (slashIndex == 0) {
        dir = "/";
    }
    else {
        dir = filename.substr(0, slashIndex);
    }

    bool ret = true;

//    // Initialize the materials
//    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++) {
//        const aiMaterial* pMaterial = pScene->mMaterials[i];

//        textures[i] = NULL;

//        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
//            aiString path;

//            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
//                std::string fullPath = dir + "/" + path.data;
//                textures[i] = new Texture(GL_TEXTURE_2D, fullPath.c_str());

//                if (!textures[i]->load()) {
//                    printf("Error loading texture '%s'\n", fullPath.c_str());
//                    delete textures[i];
//                    textures[i] = NULL;
//                    ret = false;
//                }
//                else {
//                    printf("Loaded texture '%s'\n", fullPath.c_str());
//                }
//            }
//        }

//        // Load a white texture in case the model does not include its own texture
//        if (!textures[i]) {
//            textures[i] = new Texture(GL_TEXTURE_2D, "./white.png");

//           ret = textures[i]->load();
//        }
//    }

    return ret;

}

//RENDER THIS SHIAT
void Object3D::draw(){

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    for (unsigned int i = 0 ; i < entries.size() ; i++) {
        glBindBuffer(GL_ARRAY_BUFFER, entries[i].vb);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)20);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].ib);

        const unsigned int materialIndex = entries[i].materialIndex;

//        if (materialIndex < textures.size() && textures[materialIndex]) {
//            textures[materialIndex]->bind(GL_TEXTURE0);
//        }

        glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

Object3D::~Object3D(){
}
