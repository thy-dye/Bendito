#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include <list>
#include <vector>
#include <unordered_map>
#include <string>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
};

// must have >3 indices
struct Face {
  std::vector<uint32_t> indices;
};

/*
 Used to translate the mesh to the GPU
*/
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


//halfedge stuff
// requirements manifold and non orientable
struct HE_Edge;
struct HE_Face;
struct HE_Vertex;
struct HalfEdge;

struct HE_Edge
{
  HalfEdge* halfedge;
  float crease;
  uint32_t used;
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
  HalfEdgeMesh();
  HalfEdgeMesh(std::vector<Vertex>& vertices, std::vector<Face>& faces);

  std::unique_ptr<GpuMesh> toGpuMesh();

private:
  uint32_t vertID;
  uint32_t edgeID;
  uint32_t faceID;
  std::list<HalfEdge> halfedges;
  std::list<HE_Vertex> vertices;
  std::list<HE_Face> faces;
  std::list<HE_Edge> edges;

  std::unordered_map<uint32_t, HE_Vertex*>  vertRef;
  std::unordered_map<uint32_t, HE_Edge*>  edgeRef;
  std::unordered_map<uint32_t, HE_Face*>  faceRef;
};

