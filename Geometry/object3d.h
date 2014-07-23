#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <GL/gl.h>
#include <vector>
#include <QVector3D>
#include <QVector2D>
#include "primitive.h"
#include "texture.h"

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>

struct Edge
{
    Edge(uint _a, uint _b)
    {
        assert(_a != _b);

        if (_a < _b)
        {
            a = _a;
            b = _b;
        }
        else
        {
            a = _b;
            b = _a;
        }
    }

    void Print()
    {
        printf("Edge %d %d\n", a, b);
    }

    uint a;
    uint b;
};

struct Neighbors
{
    uint n1;
    uint n2;

    Neighbors()
    {
        n1 = n2 = (uint)-1;
    }

    void addNeighbor(uint n)
    {
        if (n1 == (uint) -1) {
            n1 = n;
        }
        else if (n2 == (uint) -1) {
            n2 = n;
        }
        else {
            assert(0);
        }
    }

    uint getOther(uint me) const
    {
        if (n1 == me) {
            return n2;
        }
        else if (n2 == me) {
            return n1;
        }
        else {
            assert(0);
        }

        return 0;
    }
};

struct CompareEdges
{
    bool operator()(const Edge& edge1, const Edge& edge2)
    {
        if (edge1.a < edge2.a) {
            return true;
        }
        else if (edge1.a == edge2.a) {
            return (edge1.b < edge2.b);
        }
        else {
            return false;
        }
    }
};


struct CompareVectors
{
    bool operator()(const aiVector3D& a, const aiVector3D& b)
    {
        if (a.x < b.x) {
            return true;
        }
        else if (a.x == b.x) {
            if (a.y < b.y) {
                return true;
            }
            else if (a.y == b.y) {
                if (a.z < b.z) {
                    return true;
                }
            }
        }

        return false;
    }
};


struct Face
{
    uint indices[3];

    uint getOppositeIndex(const Edge& e) const
    {
        for (uint i = 0 ; i < sizeof(indices)/sizeof(indices[0]) ; i++) {
            uint index = indices[i];

            if (index != e.a && index != e.b) {
                return index;
            }
        }

        assert(0);

        return 0;
    }
};

struct Vertex
{
    QVector3D m_pos;
    QVector2D m_tex;
    QVector3D m_normal;

    Vertex() {}

    Vertex(const QVector3D& pos, const QVector2D& tex, const QVector3D& normal)
    {
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
    }
};


#define INVALID_MATERIAL 0xFFFFFFFFF
#define INVALID_VALUE 0xffffffff

struct MeshEntry{
    MeshEntry(){
        numIndices=0;
        baseVertex=0;
        baseIndex=0;
        materialIndex = INVALID_MATERIAL;

    }
        unsigned int numIndices;
        unsigned int baseVertex;
        unsigned int baseIndex;
        unsigned int materialIndex;
    };


class Object3D : public Primitive
{

public:
    Object3D();
    ~Object3D();
	void draw();
    void draw(QOpenGLShaderProgram *sh);
    bool loadMesh(const std::string& filename, bool withAdjacencies);

private:

    bool initFromScene(const aiScene* pScene, const std::string& filename);
    bool initMesh(const aiMesh* paiMesh, std::vector<QVector3D> &positions, std::vector<QVector3D> &normals, std::vector<QVector2D> &texCoords, std::vector<uint> &indices);
    bool initMaterials(const aiScene* pScene, const std::string& filename);
    void findAdjacencies(const aiMesh* paiMesh, std::vector<unsigned int> indices);
    void clear();

    std::map<Edge, Neighbors, CompareEdges> indexMap;
    std::map<aiVector3D, uint, CompareVectors> posMap;
    std::vector<Face> uniqueFaces;
    bool withAdjacencies;

    const aiScene* pScene;
    Assimp::Importer importer;

enum VB_TYPES {
    INDEX_BUFFER,
    POS_VB,
    NORMAL_VB,
    TEXCOORD_VB,
    NUM_VB
};

    GLuint vao;
    GLuint buffers[NUM_VB];


std::vector<MeshEntry> entries;
std::vector<Texture*> textures;

};

#endif // OBJECT3D_H
