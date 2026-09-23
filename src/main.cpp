#include "window/window.hpp"
#include "shaders/shader.hpp"
#include "application/application.hpp"
#include <string>
#include <memory>
#include "geometry/object.hpp"
#include "error.hpp"
#include "geometry/mesh.hpp"
#include <glm/gtc/type_ptr.hpp>

#define SHADER_PATH "C:/Users/Ty/Downloads/GitHub/OpenGL-First-Project/src/shaders/"

int main()
{
    Window w{WIDTH, HEIGHT, MAJOR, MINOR};
    Camera c{glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f)};
    Application RomanCancel{w, c};

    // example cube for halfedge mesh test
    

    // shader creation
    std::string Path{SHADER_PATH};
    
    std::string vert{Path + "main.vert"};
    std::string frag{Path + "main.frag"};
    Shader s(vert.c_str(), frag.c_str());
    
    auto o = std::make_unique<Object>();

    RomanCancel.addObject(o);
    RomanCancel.renderLoop();
}