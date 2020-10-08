#pragma once

#include <cstddef>

class Allocator
{
public:
    Allocator();
    ~Allocator();

    void makeAllocator(size_t maxSize);
    char* alloc(size_t size);
    void reset();

private:
    size_t buffSize;
    char* buff;
    size_t curSize;
};