/// @file page.c
/// @brief This file contains implementations related to wikipedia pages.
///
/// @author Ben Potter
/// @date 20 May 2024

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h" // used for logging and random

#include "fetch.h"
#include "page.h"


bool page_create(page *p, const char *url) {
    if(p == NULL)
        return false;

    // TODO Check if a page with this configuration already exists. 
    
    p->status = INVALID;

    // Allocate and copy the url.
    size_t link_bytes = strlen(url) + 1;
    p->url = (char*) malloc(sizeof(char) * link_bytes);
    strcpy(p->url, url);
    
    p->size_links = 0;
    p->links = NULL;
    
    return true;
}


bool page_load(page *p) {
    if(p == NULL)
        return false;

    if(p->status == LOADED)
        return false;

    // Prepare buffer for HTML data.
    page_html *response = (page_html*) malloc(sizeof(page_html));
    page_html_create(response);

    // Send request to wikipedia for the page HTML.
    bool success;
    success = page_html_fetch(response, p->url);    

    if(!success) {
        TraceLog(LOG_ERROR, "Could not create page.");

        page_html_destroy(response);
        return false;
    }

    // Extract the title and links from the page's HTML.
    page_html_extract_title(response); 
    page_html_extract_links(response);

    // Set the page title.
    strcpy(p->title, response->title);

    // Allocate memory for the child pages.
    p->size_links = response->size_links;
    p->links = (page**) malloc(sizeof(page**) * p->size_links);

    for(int i = 0; i < response->size_links; ++i) {

        // Create a page with the URL extracted from the HTML tree.
        page *child = (page*) malloc(sizeof(page));
        p->links[i] = child;

        // TODO Handle failure.
        const char *child_url = response->links[i];
        page_create(child, child_url);
    }

    p->status = LOADED;

    // Deallocate memory for links and response object.
    page_html_destroy(response);

    return true;
}


bool page_load_children(page *p) {

    for(int i = 0; i < p->size_links; ++i) {
        page *child = p->links[i]; 

        // Load the child page from wikipedia.
        page_load(child);
    }

    return true;
}


bool page_destroy(page *p) {
    if(p == NULL)
        return false;

    // TraceLog(LOG_DEBUG, "Destroying page with %d links.", p->size_links);

    // Deallocate all children if they exist.
    for(int i = 0; i < p->size_links; ++i) {
        page_destroy(p->links[i]);
    }

    // Deallocate page memory.
    free(p->url);
    free(p->links);
    free(p);
    
    return true;
}

