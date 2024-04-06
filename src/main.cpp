// Written by Ben.

#include <iostream>
#include <regex>


// everything.curl.dev
#include <curl/curl.h>

#include "fetcher.h"
#include "window.h"

#define VERSION "v0.1"
#define TARGET_URL "https://en.wikipedia.org/wiki/Cock"


int main(int argc, char* argv[]) {
    std::cout << "wikipedia scraper " << VERSION << std::endl;

    curl_global_init(CURL_GLOBAL_ALL);

    Window *window = new Window("Wikipedia Scraper");

    if(!window->init())
       return 1; 

    window->run();

    curl_global_cleanup();

    return 0;
}

