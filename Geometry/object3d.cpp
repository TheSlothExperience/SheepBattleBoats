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

    m_Entries.resize(pScene->mNumMeshes);
    m_Textures.resize(pScene->mNumMaterials);

    //Init the meshes in the scene
    for(unsigned int i = 0; i<m_Entries.size();i++){
        const aiMesh* paiMesh = pScene->mMeshes[i];
        initMesh(i,paiMesh);
    }
    //after init go further to materials
    return initMaterials(pScene, filename);
}

bool Object3D::initMesh(unsigned int index, const aiMesh *paiMesh){

    m_Entries[index].materialIndex = paiMesh->mMaterialIndex;
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

    m_Entries[index].init(vertices, indices);

}

bool Object3D::initMaterials(const aiScene *pScene, const std::string &filename){



}

//RENDER THIS SHIAT
void Object3D::draw(){

}

Object3D::~Object3D(){


}


