#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <memory>
#include <glm/glm.hpp>
#include "../all.hpp"
#include "../window/window.hpp"
#include "../geometry/object.hpp"
#include "../geometry/mesh.hpp"
#include "../camera/camera.hpp"

enum RenderContextState { WORLDSPACE, OBJECTSPACE };

class RenderContext
{
public:
    RenderContext(Window& window, Camera& camera, int frameLimit=180);
    ~RenderContext();

    RenderContext(RenderContext&) = delete;
    RenderContext& operator=(const RenderContext&) = delete; 
    RenderContext(RenderContext&&) = delete;
    RenderContext& operator=(RenderContext&&) = delete; 

    //may need to add objects in such a way to utilize unique_ptr
    void addObject(std::unique_ptr<Object> o);
    void renderLoop();

//getter functions if i have more internal variables
    double deltaTime()               { return dt; };
    RenderContextState getState()    { return contextState; }
    bool getCameraMode()             { return cameraMode; }

private:
    // private member functions
    void render();
    void cameraInputs(bool cm);
    void processInputs();

    // window object
    Window *window;
    
    //object data for the scene
    uint32_t rootID;
    std::unordered_map<uint32_t, std::shared_ptr<Object>> objects;

    // framedata
    float lastFrameTime = 0, 
    dt = 0;
    int frameLimit;
    RenderContextState contextState = WORLDSPACE;

    // data for the camera usage
    // many functions rely on cameraMode so if the type changes to support more modes change those as well
    bool cameraMode = FLYMODE;
    Camera camera;
    PerspectiveMode perspectiveMode = PERSPECTIVE;
    glm::vec3 dir;
    float yaw=-90.0f, pitch=0; 
    float mouseSens  = 1.0f,
          scrollSens = 2.0f;


};
//todo add a sleep functionality and a framerate cap