// Written by Ben.

#include <iostream>
#include <regex>


// everything.curl.dev
#include <curl/curl.h>

#include "fetcher.h"
#include "window.h"

#define VERSION "v0.1"
#define TARGET_URL "https://en.wikipedia.org/wiki/C%2B%2B"


int main(int argc, char* argv[]) {
    std::cout << "wikipedia scraper " << VERSION << std::endl;

    curl_global_init(CURL_GLOBAL_ALL);

    std::string text;

    Fetcher::fetch(TARGET_URL, text);
    Fetcher::dump("article.html", text);
    std::cout << "read: " << text.size() << " bytes" << std::endl;

    std::string title;
    std::vector<std::string> links;

    title = Fetcher::findTitle(text); 
    links = Fetcher::findLinks(text);
    std::cout << "title: " << title << " has " << links.size() << " links" << std::endl;

    Window *window = new Window("Wikipedia Scraper");

    if(!window->init())
       return 1; 

    window->run();

    curl_global_cleanup();

    return 0;
}

