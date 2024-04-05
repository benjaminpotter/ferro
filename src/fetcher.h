// Written by Ben.

#include <vector>

#pragma once

struct MemoryChunk {
    char *memory;
    size_t size;
};

class Fetcher {
public:
    static void fetch(const char* url, std::string& content);
    static void dump(const char* filepath, std::string content);
    static std::string findTitle(std::string html);
    static std::vector<std::string> findLinks(std::string html);
};
