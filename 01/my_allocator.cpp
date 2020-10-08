#include "my_allocator.h"

Allocator::Allocator()
    : buffSize(0)
    , buff(nullptr)
    , curSize(0)
{
}

Allocator::~Allocator() {
    if (buff != nullptr) {
        delete[] buff;
    }
}


void Allocator::makeAllocator(size_t maxSize) {
    if (buff != nullptr) {
        delete[] buff;
    }
    buff = new char[maxSize];
    buffSize = maxSize;
    curSize = 0;
}

char* Allocator::alloc(size_t size) {
    if ((buff == nullptr) || (size + curSize > buffSize)) {
        return nullptr;
    }
    size_t oldSize = curSize;
    curSize += size;
    return buff + oldSize;
}

void Allocator::reset() {
    curSize = 0;
}