#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>
#include "mesh.hpp"
#include "../shaders/shader.hpp"
#include "../all.hpp"

struct Transform
{
    //local space information
    glm::vec3 pos = {0.0f, 0.0f, 0.0f};
    glm::quat rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 scale = {1.0f, 1.0f, 1.0f};
    //global space information
    glm::mat4 modelMatrix = glm::mat4(1.0f);
};

class Object {
public:

Object();
Object(std::unique_ptr<HalfEdgeStructure>& tm, std::shared_ptr<Material>& m);
// for debugging purposes
Object(std::unique_ptr<GpuMesh>& gm, std::shared_ptr<Material>& m);

void activateShader() { mat->use(); }
void deactivateShader() { mat->disuse(); }
void Render();

// getters
uint32_t getID() { return ID; }

// public positional values
Transform T;

// data related to the mesh and shader
std::unique_ptr<HalfEdgeStructure> topoMesh;
std::unique_ptr<GpuMesh> gpuMesh;
std::shared_ptr<Material> mat;

// information to point to parents and children
std::weak_ptr<Object> parent;
std::vector<std::weak_ptr<Object>> children;

private:
uint32_t ID;
static uint32_t s_ID; // used to assign a unique id
};
