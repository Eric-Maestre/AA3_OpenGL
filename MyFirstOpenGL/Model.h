#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glew.h>

class Mesh {
public:
    Mesh(const std::vector<float>& vertexs, const std::vector<float>& uvs, const std::vector<float>& normals);
    void Render() const;

private:
    GLuint VAO, VBO, uvVBO, normalsVBO;
    unsigned int numVertexs;
};

#endif
