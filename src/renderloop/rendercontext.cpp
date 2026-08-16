#include "rendercontext.hpp"

/****************************************************************************
Constructor and Destructor
*****************************************************************************/

RenderContext::RenderContext(Window& window, Camera& camera, int frameLimit) : frameLimit{frameLimit} , camera{camera}
{
    this->window = &window;
    std::vector v = {GLFW_KEY_ESCAPE, GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_TAB};
    window.addInput(v);
}

//todo as we continually update this class
RenderContext::~RenderContext()
{

}

/****************************************************************************
Member Functions
*****************************************************************************/

void RenderContext::addObject(std::unique_ptr<Object> o)
{
    objects.push_back(std::move(o));
}

/***************** MAIN LOOP *****************/
void RenderContext::renderLoop()
{
    while(!window->shouldClose()) 
    {
        // get time frame
        dt = glfwGetTime() - lastFrameTime;
        lastFrameTime = glfwGetTime();

        auto map = window->getInputMap();

        if (!ImGui::GetIO().WantCaptureKeyboard) { 
            processInputs();
            window->clearInputs();
        }

        if (!ImGui::GetIO().WantCaptureMouse) { 
            window->processMouse();
            if (window->isFocused()) { //check if window is focused
                window->setDisabledCursor();
                cameraInputs(cameraMode);
            }
            else {
                window->setNormalCursor();
            }
            window->clearMouseButtons();
            window->clearMouseScroll();
        }
        glfwPollEvents();
        render();
    }
}

/****************************************************************************
Private functions
*****************************************************************************/

// actual fucntion to render
void RenderContext::render()
{
    //opengl rendering
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);  
    //view matrix
    glm::mat4 view = camera.lookAt();
    // projections
    glm::mat4 projection = camera.perspectiveProjection();
    
    for (int i = 0; i < static_cast<int>(objects.size()); ++i)
    {
    
        //model matrix
        glm::mat4 model = glm::mat4(1.0f);
        
        objects[i]->mat->use();

        objects[i]->mat->s->setMat4("model", model);
        objects[i]->mat->s->setMat4("view", view);
        objects[i]->mat->s->setMat4("projection", projection);    
        objects[i]->Render();
    }
    
    //imgui ui demo
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow();
    
    ImGui::Render();
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        // we HAVE to reset the glfw context idk why but we have to
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    window->swapBuffer();
}
//handles the camera movement takes in current camera mode
//todo make a camera controller class
void RenderContext::cameraInputs(bool cm) 
{
    if (cm == FLYMODE) {
        yaw   += window->getMouse().xoffset * 0.1f;  ///error has something to do with the permanent offset or something like that
        pitch += window->getMouse().yoffset * 0.1f; 
        if(pitch > 89.0f)  { pitch =  89.0f; }
        if(pitch < -89.0f) { pitch = -89.0f; } 
        dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        dir.y = sin(glm::radians(pitch));
        dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        camera.forward = glm::normalize(dir);
    }
    else {
        err::log(INFO, "not implemented yet");
    }
    err::log(INFO, std::to_string(window->getMouse().yscrolloffset) + " yscroll offset");
    camera.fov -= window->getMouse().yscrolloffset * scrollSens; // decrease the fov so the object looks larger
    if (camera.fov < 1.0f)  { camera.fov = 1.0f; } 
    if (camera.fov > 90.0f) { camera.fov = 90.0f; }
}
// handles all inputs
void RenderContext::processInputs()
{
        auto map = window->getInputMap();
        [[maybe_unused]]int mods = window->getModifiers();

        if (map[GLFW_KEY_ESCAPE]) { 
            window->setClose(); 
        }
        if (window->getKeyState(GLFW_KEY_W)) { camera.pos += camera.cameraSpeed * camera.getForward() * dt; } 
        if (window->getKeyState(GLFW_KEY_S)) { camera.pos += camera.cameraSpeed * -camera.getForward() * dt; }
        if (window->getKeyState(GLFW_KEY_D)) { camera.pos += camera.cameraSpeed * camera.getRight() * dt; }
        if (window->getKeyState(GLFW_KEY_A)) { camera.pos += camera.cameraSpeed * -camera.getRight() * dt; }
        
        if (map[GLFW_KEY_TAB] == GLFW_PRESS) { 
            cameraMode = !cameraMode; 
        }

}