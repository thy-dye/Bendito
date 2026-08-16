#include "mesh.hpp"

/*
GPU Mesh functions
*/
GpuMesh::GpuMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) 
{
    this->vertices = vertices; 
    this->indices = indices;
    setup_mesh();
};

void GpuMesh::setup_mesh()
{
    glGenBuffers(1, &VBO); 
    glGenBuffers(1, &EBO); 
    glGenVertexArrays(1, &VAO); 

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_DYNAMIC_DRAW); 
    
    //vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0 );

    //vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    glBindVertexArray(0);
}

void GpuMesh::Draw() 
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0)
}