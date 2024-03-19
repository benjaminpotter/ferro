// Written by Ben.

#pragma once

struct MemoryChunk {
    char *memory;
    size_t size;
};

class Fetcher {
public:
    static void fetch(const char* url, std::string& content);
    static void dump(const char* filepath, std::string content);
};
