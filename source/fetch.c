/// @file fetch.c
/// @brief This file contains implementations related to grabbing and parsing 
/// HTML pages from wikipedia.
///
/// @author Ben Potter
/// @date 20 May 2024

#include <stdlib.h>
#include <string.h>

#include "raylib.h" // for logging
#include <curl/curl.h>

#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "fetch.h"


struct memory_chunk {
    char *memory;
    size_t size;
};


bool page_html_create(page_html *html) {

    html->bytes = 0;
    html->html = NULL;

    // Set first byte to NULL byte to indicate this is a NULL string.
    html->title[0] = '\0';

    html->size_links = 0;
    html->links = NULL;
    
    return true;
}


// Callback function for handling incoming HTTP data.
static size_t handle_chunk(void *contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    struct memory_chunk *mem = (struct memory_chunk *)userp;

    mem->memory = (char*) realloc(mem->memory, mem->size + realsize + 1);

    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


bool page_html_fetch(page_html *html, const char *url) {
    if(html == NULL)
        return false;

    // Default to error state unless otherwise set.
    bool err = true;

    CURL* curl_handle;
    CURLcode res;
    
    // TODO Integrate the memory_chunk with the page_html strcuture.
    struct memory_chunk chunk;
    
    curl_handle = curl_easy_init();

    chunk.memory = (char*) malloc(1);
    chunk.size = 0;

    TraceLog(LOG_INFO, "Fetching %s", url);
    
    // Set request options.
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, handle_chunk);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void*) &chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // Perform request.
    res = curl_easy_perform(curl_handle);

    // If the request is successful and returned data, copy the HTML data.
    if(res == CURLE_OK && chunk.size != 0) {
        err = false;

        // Set number of bytes in HTML.
        html->bytes = chunk.size;

        // Copy the HTML from the buffer.
        html->html = (char*) malloc(html->bytes);
        memcpy(html->html, chunk.memory, html->bytes);
    }
    
    // Perform clean up.
    free(chunk.memory);
    curl_easy_cleanup(curl_handle);
    return !err;
}


bool page_html_extract_title(page_html *html) {
    if(html == NULL)
        return false;

    bool err = false;
    xmlInitParser();

    // TODO Some titles are italic so they have an extra <i> tag. Could use
    // the | operator to also select those.

    // XPath expression used to select the title of a wikipedia page.
    const xmlChar* title_xpath = 
        (const xmlChar*) "/html/body/div[2]/div/div[3]/main/header/h1/span";

    htmlDocPtr doc;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr nodes;

    // Parse the raw characters into an HTML tree.
    doc = htmlReadMemory(html->html, html->bytes, NULL, NULL, HTML_PARSE_NOERROR);
    if(doc == NULL) {
        TraceLog(LOG_ERROR, "Failed to parse HTML.");

        // No memory was allocated by htmlReadMemory.
        goto cleanup;
        err = true;
    }

    // Create new context for XPath search.
    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        TraceLog(LOG_ERROR, "Failed to create XML context.");

        goto cleanup_doc;
        err = true;
    }


    // Evaluate the XPath over the HTML tree.
    obj = xmlXPathEvalExpression(title_xpath, ctx);
    if(obj == NULL) {
        TraceLog(LOG_ERROR, "Failed to evaluate XPath.");

        goto cleanup_ctx;
        err = true;
    }


    // Collect the returned nodes.
    nodes = obj->nodesetval;
    if(nodes == NULL || nodes->nodeTab == NULL) {
        TraceLog(LOG_ERROR, "Failed to parse title from HTML.");

        strcpy(html->title, "ENOTITLE");
        goto cleanup_obj;
        err = true;
    }

    // Select the content of the first node that matches.
    xmlChar *content = xmlNodeGetContent(nodes->nodeTab[0]);
    TraceLog(LOG_INFO, "Parsed title %s", content);
    strcpy(html->title, (char*) content);
    xmlFree(content);

cleanup_obj:
    xmlXPathFreeObject(obj);
cleanup_ctx:
    xmlXPathFreeContext(ctx);
cleanup_doc:
    xmlFreeDoc(doc); 
cleanup:
    xmlCleanupParser();
    return !err;
}


bool page_html_extract_links(page_html *html) {
    if(html == NULL)
        return false;

    bool err = false;
    xmlInitParser();

    // XPath expression used to select the links of a wikipedia page.
    const xmlChar* link_xpath = (const xmlChar*) 
        "//div[@class='mw-body-content']"
        "//a[starts-with(@href,'/wiki/')"
        "and not(starts-with(@href, '/wiki/File:'))"
        "and not(starts-with(@href, '/wiki/Wikipedia:'))"
        "and not(starts-with(@href, '/wiki/Special:'))"
        "and not(starts-with(@href, '/wiki/Help:'))"
        "]/@href";

    htmlDocPtr doc;
    xmlXPathContextPtr ctx;
    xmlXPathObjectPtr obj;
    xmlNodeSetPtr nodes;

    // Parse the raw characters into an HTML tree.
    doc = htmlReadMemory(html->html, html->bytes, NULL, NULL, HTML_PARSE_NOERROR);
    if(doc == NULL) {
        TraceLog(LOG_ERROR, "Failed to parse HTML.");

        // No memory was allocated by htmlReadMemory.
        goto cleanup;
        err = true;
    }

    // Create new context for XPath search.
    ctx = xmlXPathNewContext(doc);
    if(ctx == NULL) {
        TraceLog(LOG_ERROR, "Failed to create XML context.");

        goto cleanup_doc;
        err = true;
    }

    // Evaluate the XPath over the HTML tree.
    obj = xmlXPathEvalExpression(link_xpath, ctx);
    if(obj == NULL) {
        TraceLog(LOG_ERROR, "Failed to evaluate XPath.");

        goto cleanup_ctx;
        err = true;
    }

    // Collect the returned nodes.
    nodes = obj->nodesetval;
    if(nodes == NULL) {
        TraceLog(LOG_ERROR, "Failed to parse links from HTML.");

        goto cleanup_obj;
        err = true;
    }

    // Allocate memory for each link.
    html->size_links = 10; // nodes->nodeNr;
    html->links = (char**) malloc(sizeof(char*) * html->size_links);

    // Loop over each link.
    for(int i = 0; i < html->size_links; ++i) {
        
        // Grab the link content from the node.
        xmlChar *xml_content = xmlNodeGetContent(nodes->nodeTab[i]);
        char *content = (char*) xml_content;

        // TODO There needs to be a better string interface...
        const char *url_prefix = "https://en.wikipedia.org";
        size_t link_bytes = strlen(url_prefix) + strlen(content) + 1;

        // Allocate space on the heap for the link text.
        html->links[i] = (char*) malloc(sizeof(char) * link_bytes);

        // Copy the link text into the newly allocated space.
        // First copying the prefix, then appending the link.
        strcpy(html->links[i], url_prefix);
        strcat(html->links[i], content);
        // TraceLog(LOG_DEBUG, "%s", html->links[i]);

        // Deallocate the xml_content string.
        xmlFree(xml_content);
    }

cleanup_obj:
    xmlXPathFreeObject(obj);
cleanup_ctx:
    xmlXPathFreeContext(ctx);
cleanup_doc:
    xmlFreeDoc(doc); 
cleanup:
    xmlCleanupParser();
    return !err;
}


bool page_html_destroy(page_html *html) {
    if(html == NULL)
        return false;

    // TODO Don't actually free the structure, it should be on the stack.

    // If there are bytes allocated to this page, free them.
    if(html->bytes != 0)
        free(html->html);

    // If there are links allocated to this page, free them.
    for(int i = 0; i < html->size_links; ++i)
        free(html->links[i]);

    // If the links array has been allocated, free it.
    if(html->links != NULL)
        free(html->links);

    free(html);

    return true;
}
