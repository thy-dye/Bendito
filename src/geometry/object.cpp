#include "object.hpp"

uint32_t Object::s_ID = 0; 

Object::Object()
: topoMesh{nullptr}, gpuMesh{nullptr}, mat{nullptr}, ID{++s_ID} {
    //figure out pos rotation and scale later
}
Object::Object(std::unique_ptr<HalfEdgeMesh>& tm, std::shared_ptr<Material>& m)
: topoMesh{std::move(tm)}, gpuMesh{std::move(topoMesh.toGpuMesh())}, mat{m}, ID{++s_ID} {
    
}
Object::Object(std::unique_ptr<GpuMesh>& gm, std::shared_ptr<Material>& m)
: topoMesh{nullptr}, gpuMesh{std::move(gm)}, mat{m}, ID{++s_ID} {
    
}

void Object::Render()
{
    gpuMesh->Draw();
}