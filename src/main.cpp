// Written by Ben.

#include <iostream>
#include <regex>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

// everything.curl.dev
#include <curl/curl.h>

// https://learnopengl.com/Getting-started/Hello-Window
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "fetcher.h"

#define VERSION "v0.1"
#define TARGET_URL "https://en.wikipedia.org/wiki/C%2B%2B"

struct Article {
    std::string url;
    std::string title;

    Article* links[];
};


int _main(int argc, char* argv[]) {
    std::cout << "wikipedia scraper " << VERSION << std::endl;
    
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    curl_global_init(CURL_GLOBAL_ALL);

    std::string text;

    Fetcher::fetch(TARGET_URL, text);
    Fetcher::dump("article.html", text);
    std::cout << "read: " << text.size() << " bytes" << std::endl;

    htmlDocPtr doc;
    htmlNodePtr cur;

    doc = htmlReadMemory(text.c_str(), text.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    if(!doc) {
        std::cerr << "failed to parse html" << std::endl;
        return 1;
    }

    const xmlChar* titleExpr = (const xmlChar*) "/html/body/div[2]/div/div[3]/main/header/h1/span";
    const xmlChar* linkExpr = (const xmlChar*) "//a[contains(@href, '/wiki/')]/@href";

    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    ctx = xmlXPathNewContext(doc);
    xmlNodeSetPtr nodes;

    obj = xmlXPathEvalExpression(titleExpr, ctx);
    nodes = obj->nodesetval;
    for(int i = 0; i < nodes->nodeNr; ++i) {
        xmlChar *content = xmlNodeGetContent(nodes->nodeTab[i]);
        std::cout << "xpath search: " << content << std::endl;
    }

    obj = xmlXPathEvalExpression(linkExpr, ctx);
    nodes = obj->nodesetval;
    for(int i = 0; i < nodes->nodeNr; ++i) {
        xmlChar *content = xmlNodeGetContent(nodes->nodeTab[i]);
        std::cout << "xpath search: " << content << std::endl;
    }
    
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    curl_global_cleanup();

    return 0;
}

int main(int argc, char* argv[]) {

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow *window = glfwCreateWindow(320, 180, "Wikipedia Scraper", NULL, NULL);
    if(!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return 1;
    }

    glfwMakeContextCurrent(window); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return 1;
    }   

    glViewport(0, 0, 320, 180); 


    while(!glfwWindowShouldClose(window))
    {
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }
        
    glfwTerminate();
    return 0;
}
