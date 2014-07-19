#define GL_GLEXT_PROTOTYPES

#include "object3d.h"
#include <vector>

#include <assert.h>

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2


Object3D::Object3D(){

    vao = 0;
    memset(buffers, 0, sizeof(buffers));
    pScene = NULL;
    withAdjacencies = false;

}

void Object3D::clear(){

    for(uint i=0; i<textures.size();i++){
        if (textures[i]) { delete textures[i]; textures[i] = NULL; }
    }

    if(buffers[0]!=0){
        glDeleteBuffers((sizeof(buffers)/sizeof(buffers[0])),buffers);
    }

    if(vao != 0){
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }

}

bool Object3D::loadMesh(const std::string& filename, bool withAdjacencies){

    this->withAdjacencies = withAdjacencies;

    //Release ma vaaderr
    clear();

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers((sizeof(buffers)/sizeof(buffers[0])),buffers);

    bool loaded = false;

    pScene = importer.ReadFile(filename.c_str(),aiProcess_Triangulate |aiProcess_GenSmoothNormals
                                              | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);
    if(pScene){

        loaded = initFromScene(pScene,filename);
    }else{
        printf("Error parsing '%s': '%s'\n", filename.c_str(), importer.GetErrorString());
    }

    glBindVertexArray(0);

    return loaded;
}


//First init all the meshes for the scene
bool Object3D::initFromScene(const aiScene *pScene, const std::string &filename){

    entries.resize(pScene->mNumMeshes);
    textures.resize(pScene->mNumMaterials);

    std::vector<QVector3D> positions;
    std::vector<QVector3D> normals;
    std::vector<QVector2D> texCoords;
    std::vector<uint> indices;


    uint numVertices = 0;
    uint numIndices = 0;

    uint verticesPerPrimitive = withAdjacencies ? 6 : 3;

    //count vertices and indices
    for(uint i = 0; i<entries.size();i++){
        entries[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
        entries[i].numIndices = pScene->mMeshes[i]->mNumFaces * verticesPerPrimitive;
        entries[i].baseVertex = numVertices;
        entries[i].baseIndex = numIndices;

        numVertices += pScene->mMeshes[i]->mNumVertices;
        numIndices += entries[i].numIndices;
    }

    positions.reserve(numVertices);
    normals.reserve(numVertices);
    texCoords.reserve(numVertices);
    indices.reserve(numIndices);

    //Init the meshes in the scene
    for(uint i = 0; i<entries.size();i++){
        const aiMesh* paiMesh = pScene->mMeshes[i];

        initMesh(paiMesh,positions,normals,texCoords,indices);
    }

    if(!initMaterials(pScene,filename)){
        return false;
    }

    glBindBuffer(GL_ARRAY_BUFFER, buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0])*positions.size(), &positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0])*texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION,2,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0])*normals.size(), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION,3,GL_FLOAT,GL_FALSE,0,0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[0])*indices.size(),&indices[0],GL_STATIC_DRAW);

    //after init go further to materials
    return true;
}

static uint getOppositeindex(const aiFace& face, const Edge& e){

    for (uint i = 0 ; i < 3 ; i++) {
        uint Index = face.mIndices[i];

        if (Index != e.a && Index != e.b) {
            return Index;
        }
    }

    assert(0);

    return 0;
}

bool Object3D::initMesh(const aiMesh *paiMesh,
                        std::vector<QVector3D>& positions,
                        std::vector<QVector3D>& normals,
                        std::vector<QVector2D>& texCoords,
                        std::vector<uint>& indices){

    const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

    //populate vertex attribute vertices
    for (unsigned int i = 0 ; i < paiMesh->mNumVertices ; i++) {
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &zero3D;

        positions.push_back(QVector3D(pPos->x, pPos->y, pPos->z));
        texCoords.push_back(QVector2D(pTexCoord->x, pTexCoord->y));
        normals.push_back(QVector3D(pNormal->x, pNormal->y, pNormal->z));
    }

    if(withAdjacencies){
        findAdjacencies(paiMesh,indices);
    }else{
        for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) {
            const aiFace& Face = paiMesh->mFaces[i];
            assert(Face.mNumIndices == 3);
            indices.push_back(Face.mIndices[0]);
            indices.push_back(Face.mIndices[1]);
            indices.push_back(Face.mIndices[2]);
        }
    }
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

    // Initialize the materials
    for (uint i = 0 ; i < pScene->mNumMaterials ; i++) {
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) {
                std::string p(Path.data);

                if (p.substr(0, 2) == ".\\") {
                    p = p.substr(2, p.size() - 2);
                }

                std::string FullPath = dir + "/" + p;

                textures[i] = new Texture(GL_TEXTURE_2D, FullPath.c_str());

                if (!textures[i]->load()) {
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete textures[i];
                    textures[i] = NULL;
                    ret = false;
                }
                else {
                    printf("%d - loaded texture '%s'\n", i, FullPath.c_str());
                }
            }
        }
    }

    return ret;

}



void Object3D::findAdjacencies(const aiMesh *paiMesh, std::vector<unsigned int> indices){

    // Step 1 - find the two triangles that share every edge
    for (uint i=0; i<paiMesh->mNumFaces;i++){
        const aiFace& face = paiMesh->mFaces[i];

        Face unique;

        //if position vector is duplicated in vb, get index of first appereance
        for(uint j=0;j<3;j++){

            uint index = face.mIndices[j];
            aiVector3D& v = paiMesh->mVertices[index];

            if(posMap.find(v)==posMap.end()){
                posMap[v]=index;
            }else{
                index = posMap[v];
            }
            unique.indices[j]=index;
        }

        uniqueFaces.push_back(unique);

        Edge e1(unique.indices[0], unique.indices[1]);
        Edge e2(unique.indices[1], unique.indices[2]);
        Edge e3(unique.indices[2], unique.indices[0]);

        indexMap[e1].addNeighbor(i);
        indexMap[e2].addNeighbor(i);
        indexMap[e3].addNeighbor(i);
    }

    // Step 2 - build the index buffer with the adjacency info
    for(uint i=0;i<paiMesh->mNumFaces;i++){
        const Face& face = uniqueFaces[i];

        for(uint j=0; j<3;j++){
            Edge e(face.indices[j], face.indices[(j+1)%3]);
            assert(indexMap.find(e) != indexMap.end());
            Neighbors n = indexMap[e];
            uint otherTri = n.getOther(i);

            assert(otherTri != -1);

            const Face& otherFace = uniqueFaces[otherTri];
            uint oppositeIndex = otherFace.getOppositeIndex(e);

            indices.push_back(face.indices[j]);
            indices.push_back(oppositeIndex);
        }
    }

}

//RENDER THIS SHIAT
void Object3D::draw(){

    glBindVertexArray(vao);
    uint topology = withAdjacencies ? GL_TRIANGLES_ADJACENCY : GL_TRIANGLES;

    for(uint i = 0; i<entries.size();i++){
        const uint materialIndex = entries[i].materialIndex;

        assert(materialIndex<textures.size());

        if(textures[materialIndex]){
            textures[materialIndex]->bind(GL_TEXTURE0);
        }

        glDrawElementsBaseVertex(topology,
                                 entries[i].numIndices,
                                 GL_UNSIGNED_INT,
                                 (void*)(sizeof(uint)*entries[i].baseIndex),
                                 entries[i].baseVertex);
    }

    glBindVertexArray(0);

}

Object3D::~Object3D(){
}
