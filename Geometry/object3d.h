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


class Object3D : public Primitive
{

public:
    Object3D();
    ~Object3D();
    void draw();
    bool loadMesh(const std::string& filename);

private:

    bool initFromScene(const aiScene* pScene, const std::string& filename);
    bool initMesh(unsigned int index, const aiMesh* paiMesh);
    bool initMaterials(const aiScene* pScene, const std::string& filename);
    void clear();

#define INVALID_MATERIAL 0xFFFFFFFFF
#define INVALID_VALUE 0xffffffff

struct MeshEntry{
        MeshEntry();
        ~MeshEntry();
        bool init(const std::vector<Vertex>& vertices,
                  const std::vector<unsigned int>& indices);

        GLuint vb;
        GLuint ib;

        unsigned int numIndices;
        unsigned int materialIndex;
    };

    std::vector<MeshEntry> m_Entries;
    std::vector<Texture> m_Textures;
};

#endif // OBJECT3D_H
