#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm.hpp>

class Mesh {
public:

    Mesh() = default;

    Mesh(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render() const;

    static Mesh LoadOBJMesh(const std::string& filePath);


private:
    GLuint VAO, VBO, uvVBO, normalsVBO;
    unsigned int numVertexs;
};

#endif
