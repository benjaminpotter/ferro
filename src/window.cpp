
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/random.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp> 

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "window.h"
#include "fetcher.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

Window::Window(const char *title): title(title) {

}

Window::~Window() {

    for(auto node : nodes)
        delete node;

    for(auto text : texts)
        delete text;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Window::initGLFW() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    if(!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }   
}

void Window::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    // io.Fonts->AddFontDefault();
    io.Fonts->AddFontFromFileTTF("data/ProggyClean.ttf", 32);

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
                                                        
    // disable persistent windowing
    io.IniFilename = nullptr;

    // disable logging
    io.LogFilename = nullptr;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Window::initShaders() {
    // TODO apply global shader context
}

void Window::initNodes() {
    
    Node *node = new Node(); 
    node->position = glm::vec3(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, 0.0f);

    std::string nodeTitle = std::string("Node");
    Text *text = new Text(nodeTitle);

    nodes.push_back(node);
    texts.push_back(text);
}

bool Window::init() {

    initGLFW();
    initImGui();
    initShaders();
    initNodes();

    // TODO check for failed init

    return true;
}

void Window::drawNodes() {

    for(unsigned int i = 0; i < nodes.size(); ++i) {
        nodes[i]->draw();
    }

    for(unsigned int i = 0; i < texts.size(); ++i) {
        texts[i]->draw();
    }

    #if 0
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR)
    {
        std::cout << "ERR::WINDOW opengl: " << err << std::endl;
    } 
    #endif
}

void Window::run() {

    static char url[128];
    
    std::string title;
    size_t size;
    int links;

    ImGui::GetStyle().ScaleAllSizes(2.0f);
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();    

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Wikipedia Search Tool");
        ImGui::Text("Search and parse metadata from any Wikipedia article.");

        ImGui::InputTextWithHint("##", "URL", url, IM_ARRAYSIZE(url));
        ImGui::SameLine(); 
        if(ImGui::Button("Search")) {
            std::string text;

            Fetcher::fetch(url, text);

            title = Fetcher::findTitle(text); 
            size = text.size();
            links = Fetcher::findLinks(text).size();
        }
        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        ImGui::Text("Title: %s", title.c_str());
        ImGui::Text("Size: %2.0f KB", (float) size / 1024.0);
        ImGui::Text("Links: %d", links);

        ImGui::End();

        ImGui::Render();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  
        glDisable(GL_DEPTH_TEST);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        drawNodes();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); 

        glfwSwapBuffers(window);
    }
}

