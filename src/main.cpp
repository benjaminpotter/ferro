// Written by Ben.

#include <iostream>
#include <regex>

// everything.curl.dev
#include <curl/curl.h>

#include "fetcher.h"

#define VERSION "v0.1"
#define TARGET_URL "https://en.wikipedia.org/wiki/C%2B%2B"


int main(int argc, char* argv[]) {
    std::cout << "wikipedia scraper " << VERSION << std::endl;
    curl_global_init(CURL_GLOBAL_ALL);
    
    std::string text;

    Fetcher::fetch(TARGET_URL, text);
    Fetcher::dump("article.html", text);
    std::cout << "read: " << text.size() << " bytes" << std::endl;
    
    std::cout << "extracting links" << std::endl;
    std::regex regex_link("\\/wiki\\/[^\"]*");        
    for(std::smatch match; std::regex_search(text, match, regex_link);) {
        std::cout << match.str() << "\n";
        text = match.suffix();
    }
   
    std::cout << std::endl;

    curl_global_cleanup();

    return 0;
}
