#include "mesh.hpp"
#include <unordered_map>

/****************************************/
/************ GPUmesh Class *************/
/****************************************/

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

/****************************************/
/********** HalfEdgeMesh Class **********/
/****************************************/

HalfEdgeMesh::HalfEdgeMesh() : vertID(0), edgeID(0), faceID(0) { }
HalfEdgeMesh::HalfEdgeMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces) 
: vertID(0), edgeID(0), faceID(0) 
{ generateMesh(vertices, faces); }

std::unique_ptr<GpuMesh> HalfEdgeMesh::toGpuMesh()
{

}

// i alone am the spinning one https://media2.giphy.com/media/v1.Y2lkPTc5MGI3NjExYXJ5c2h6a2RjdXprNDQ1eXo0bzd6aWtzZXVvbTM4YTNsNDhhdzhoZSZlcD12MV9pbnRlcm5hbF9naWZfYnlfaWQmY3Q9Zw/aU5EJyo9jP7RMvDrV9/giphy.gif
// supports orientable, non-closed 2-manifold objects
bool HalfEdgeMesh::generateMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces) {

    // used for constructing cyclic nature of halfedge along with twins
    std::unordered_map<std::pair<uint32_t, uint32_t>, HalfEdge*> edges;
    // used to reference Vertex vertices with HE_Vertex vertices
    std::unordered_map<uint32_t, uint32_t> V_to_HEV;            

    for (const auto& face : faces) 
    {
        // create a new HE_Face and add it to the map
        HE_Face* f = &this->faces.emplace_back(nullptr, faceID);
        faceRef[faceID++] = f;

        // get all vertices of a Face and turn it into a list of edges
        size_t numVerts = face.indices.size();

        // first loop over edges
        HE_Vertex* prevVert = nullptr;
        HE_Vertex* currVert = nullptr;
        HE_Vertex* v;
        for (int i = 0; i < numVerts; ++i) // loop creates vertices and halfedges
        {
            // create Vertex and add to the map or get it's reference
            int vertIdx = face.indices[i % numVerts];
            if (V_to_HEV.contains(vertIdx)) {
                v = vertRef[V_to_HEV[vertIdx]];
            }
            else {
                v = &this->vertices.emplace_back(nullptr, vertices[vertIdx], vertID);
                vertRef[vertID++] = v;
                V_to_HEV[vertIdx] = v->ID;
            }
            currVert = v; 

            // if not the first vertex - make a halfedge and store it
            if (prevVert != nullptr) {
                HalfEdge* he = &halfedges.emplace_back(nullptr, nullptr, nullptr,
                                                       nullptr, prevVert, f);
                edges[std::make_pair(prevVert->ID, currVert->ID)] = he;
            }
            prevVert = currVert;
        }

        // second loop over edges
        prevVert = nullptr;
        currVert = nullptr;
        auto fstEdge = std::make_pair(vertRef[V_to_HEV[0]]->ID, vertRef[V_to_HEV[numVerts - 1]]->ID);
        HalfEdge* prevHE = edges[fstEdge];
        HalfEdge* currHE = nullptr;
        for (int i = 0; i < numVerts; ++i) 
        {
            // get vertex reference
            int vertIdx = face.indices[i % numVerts];
            v = vertRef[V_to_HEV[vertIdx]];
            currVert = v;

            // connect edges if not first vert
            if (prevVert != nullptr) {
                // connect halfedges
                currHE = edges[std::make_pair(prevVert->ID, currVert->ID)];
                prevHE->next = currHE;
                currHE->prev = prevHE;
                
                // if opposite halfedge exists connect them as twins
                if (edges.contains(std::make_pair(currVert->ID, prevVert->ID))) {
                    currHE->twin = edges[std::make_pair(currVert->ID, prevVert->ID)];
                    edges[std::make_pair(currVert->ID, prevVert->ID)]->twin = currHE;
                }
                prevHE = currHE;
            }
            
            prevVert = currVert;
            
        }

    } 
}