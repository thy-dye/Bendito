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

enum ApplicationState { WORLDSPACE, OBJECTSPACE };

class Application
{
public:
    /*** Constructors ***/
    Application(Window& window, Camera& camera, int frameLimit=180);
    ~Application();

    Application(Application&) = delete;
    Application& operator=(const Application&) = delete; 
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete; 

    /*** Member Functions ***/
    void addObject(std::unique_ptr<Object>& o);
    void renderLoop();

    /*** getter functions ***/
    double deltaTime()          { return dt; };
    ApplicationState getState() { return contextState; }
    bool getCameraMode()        { return cameraMode; }

private:
    /*** private member functions ***/
    void render();
    void cameraInputs(int cm);
    void processInputs();
    
    /*** Data Members ***/
    Window *window;

    // Scene/Object data
    uint32_t rootID;
    std::unordered_map<uint32_t, std::unique_ptr<Object>> objects;

    // frame data
    float lastFrameTime = 0, 
    dt = 0;
    int frameLimit;
    ApplicationState contextState = WORLDSPACE;

    // camera data
    bool cameraMode = FLYMODE;
    Camera camera;
    PerspectiveMode perspectiveMode = PERSPECTIVE;
    glm::vec3 dir;
    float cameraSpeed = 1.5f;
    float yaw=-90.0f, pitch=0; 
    // mouse data
    float mouseSens  = 1.0f,
          scrollSens = 2.0f;
};
//todo add a sleep functionality and a framerate cap