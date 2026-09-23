#include "mesh.hpp"
#include <unordered_set>

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
    glBindVertexArray(0);
}

/* 
Halfedge Mesh functions
*/
HalfEdgeMesh::HalfEdgeMesh() 
{

}

HalfEdgeMesh::HalfEdgeMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces) 
{    
    // global information to check edges and vertices
    std::vector<uint32_t> vertexIDs;
    vertexIDs.resize(vertices.size(), -1);
    std::unordered_set<std::pair<uint32_t, uint32_t>> edges;

    // local information created on the fly
    std::vector<std::pair<uint32_t, uint32_t>> edgesOfFace;            

    for (const auto& face : faces) 
    {
        // creating edge list
        uint32_t prev = -1;
        for (int i=0; i < face.indices.size(); ++i)
        {
            // create vertex, add to map, add to id to an index
            uint32_t index = face.indices[i];
            HE_Vertex listVertRef = this->vertices.emplace_back(nullptr, vertices[index], vertID++);
            vertRef[vertID] = &listVertRef;
            vertexIDs[index] = vertID;
            if (i != 0) 
            {
                edges.emplace_back(prev, index);
            }
            prev = index
        }
        edges.emplace_back(prev, face.indices[0]);

        for (const auto& edge : edges) {

        }

        for (const auto& edge : edges) {
            
        }

        HalfEdge* prev = nullptr;
        uint32_t prevID = -1;
        
        halfedges.emplace_back(nullptr);

        prevID = i;
        HE_Face listFaceRef = this->faces.emplace_back(nullptr, faceID++);
    } 
}

std::unique_ptr<GpuMesh> HalfEdgeMesh::toGpuMesh()
{

}