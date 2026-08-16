#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
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

struct halfedgeVertex 
{
  glm::vec3 coordinate;
  HalfEdge* IncidentEdge;
};

struct Face 
{
  HalfEdge* halfedge;
};

struct HalfEdge 
{
  HalfEdge* twin;
  HalfEdge* next;
  HalfEdge* prev;
  Vertex* origin;
  Face* face;
};


/*
 Used to actually compute operations and change mesh
*/
class HalfEdgeStructure {
public:
  HalfEdgeStructure();

  std::unique_ptr<GpuMesh> toGpuMesh();
private:
  
};

