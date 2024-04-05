// Written by Ben.

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

// everything.curl.dev
#include <curl/curl.h>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "fetcher.h"

static size_t handle_chunk(void *contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct MemoryChunk *mem = (struct MemoryChunk *)userp;

    mem->memory = (char*) realloc(mem->memory, mem->size + realsize + 1);

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


void Fetcher::fetch(const char* url, std::string& content) {
    CURL* curl_handle;
    CURLcode res;
    
    struct MemoryChunk chunk;
    
    curl_handle = curl_easy_init();

    chunk.memory = (char*) malloc(1);
    chunk.size = 0;
    
    // set request options
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, handle_chunk);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*) &chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        std::cerr << "failed: " << curl_easy_strerror(res) << std::endl;
        throw;
    }
    
    content = chunk.memory;

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
}


void Fetcher::dump(const char* filepath, std::string content) {
    std::ofstream file;

    file.open(filepath);
    if(file) {
        file << content;
        file.close();
    }
}


std::string Fetcher::findTitle(std::string html) {
    htmlDocPtr doc;
    std::string title;

    doc = htmlReadMemory(html.c_str(), html.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    if(!doc) {
        std::cerr << "failed to parse html" << std::endl;
        return title;
    }

    const xmlChar* titleExpr = (const xmlChar*) "/html/body/div[2]/div/div[3]/main/header/h1/span";

    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    ctx = xmlXPathNewContext(doc);
    xmlNodeSetPtr nodes;

    obj = xmlXPathEvalExpression(titleExpr, ctx);
    nodes = obj->nodesetval;
    for(int i = 0; i < nodes->nodeNr; ++i) {
        xmlChar *content = xmlNodeGetContent(nodes->nodeTab[i]);
        title = std::string(reinterpret_cast<char*>(content));
    }

    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);
    
    return title;
}


std::vector<std::string> Fetcher::findLinks(std::string html) {
    htmlDocPtr doc;
    std::vector<std::string> links;

    doc = htmlReadMemory(html.c_str(), html.length(), nullptr, nullptr, HTML_PARSE_NOERROR);

    if(!doc) {
        std::cerr << "failed to parse html" << std::endl;
        return links;
    }

    const xmlChar* linkExpr = (const xmlChar*) "//a[contains(@href, '/wiki/')]/@href";

    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;

    ctx = xmlXPathNewContext(doc);
    xmlNodeSetPtr nodes;

    obj = xmlXPathEvalExpression(linkExpr, ctx);
    nodes = obj->nodesetval;
    for(int i = 0; i < nodes->nodeNr; ++i) {
        xmlChar *content = xmlNodeGetContent(nodes->nodeTab[i]);
        links.push_back( std::string( reinterpret_cast<char*>(content) ) );
    }
    
    xmlXPathFreeContext(ctx);
    xmlFreeDoc(doc);

    return links;
}
