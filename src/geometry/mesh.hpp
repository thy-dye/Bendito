#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include <iterator>
#include <type_traits>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 UV;
};

// must have >3 indices
struct Face {
  std::vector<uint32_t> indices;
};

/****************************************/
/************ GPUmesh Class *************/
/****************************************/

class GpuMesh {
public:
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  
  GpuMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);

  void Draw();

private:
  void setup_mesh();
  unsigned int VAO, VBO, EBO;

};


/****************************************/
/********** HalfEdgeMesh Class **********/
/****************************************/

// halfedge stuff
// requires manifold and non orientable
struct HE_Edge;
struct HE_Face;
struct HE_Vertex;
struct HalfEdge;

struct HE_Edge
{
  HalfEdge* halfedge;
  float crease;
  uint32_t ID;
};
struct HE_Face
{
  HalfEdge* halfedge;
  uint32_t ID;
};
struct HE_Vertex
{
  HalfEdge* halfedge;
  Vertex vert;
  uint32_t ID;
};
struct HalfEdge 
{
  HalfEdge* twin;
  HalfEdge* next;
  HalfEdge* prev;

  HE_Edge* edge;
  HE_Vertex* origin;
  HE_Face* face;
};


/*
 Used to actually compute operations and change mesh
 Mesh is always manifold and could be non-orientable
*/
class HalfEdgeMesh {
public:
  // constructors 
  HalfEdgeMesh();
  HalfEdgeMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces);

  // member functions
  std::unique_ptr<GpuMesh> toGpuMesh();
  
  bool validateMesh();

  // iterators
  template <bool Const>
  struct Iterator;
  Iterator<false> begin();
  Iterator<false> end();
  Iterator<true> cbegin();
  Iterator<true> cend();
private:
  //private member functions
  bool generateMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces);
  
  //private member data
  //uuids
  uint32_t vertID;
  uint32_t edgeID;
  uint32_t faceID;
  //all the data for the mesh
  std::list<HalfEdge> halfedges;
  std::list<HE_Vertex> vertices;
  std::list<HE_Face> faces;
  std::list<HE_Edge> edges;
  //uuids to datatypes
  std::unordered_map<uint32_t, HE_Vertex*>  vertRef; 
  std::unordered_map<uint32_t, HE_Edge*>  edgeRef;
  std::unordered_map<uint32_t, HE_Face*>  faceRef;
};

// iterators for iterating over each element over the entire Mesh
typedef   std::list<HalfEdge>::iterator HEM_halfedges;
typedef  std::list<HE_Vertex>::iterator HEM_vertices;
typedef    std::list<HE_Face>::iterator HEM_faces;
typedef    std::list<HE_Edge>::iterator HEM_edges;

/*************ITERATORS for HALFEDGE MESH************/
// custom iterator this is for each Halfedge in a face
template <bool Const>
struct HalfEdgeMesh::Iterator
{
  Iterator(HE_Face* face) : start(false), curr{face->halfedge} {};
  
  //prefix
  Iterator& operator++();
  Iterator& operator--();
  //postfix
  Iterator& operator++(int);
  Iterator& operator--(int);

  template <bool Q = Const>
  typename std::enable_if<Q, HalfEdge&>::type operator*() { return *curr; };
  template <bool Q = Const>
  typename std::enable_if<!Q, const HalfEdge&>::type operator*() { return *curr; };

  bool operator==(const Iterator& iter) { return this->curr==iter.curr;};
  bool operator!=(const Iterator& iter) { return this->curr==iter.curr;};
private:
  bool start;
  HalfEdge* curr;
};

// Namespace for any extra functions I may need
namespace HE {
// hashing for the Edges where each vertex is it's id
struct EdgeHasher
{
  std::size_t operator()(const std::pair<uint32_t, uint32_t>& e) const
  {
    using std::size_t;
    using std::hash;

    // Compute individual hash values for first and second then combine them using XOR
    // assumption of a 64 bit system
    // https://www.quora.com/What-is-a-good-hash-function-for-a-pair-of-integers
    size_t h = (size_t(e.first)<<32)+size_t(e.second); 
    h *= 1231231557ull; 
    h ^= (h>>32); 
    return h; 
  }
};

}