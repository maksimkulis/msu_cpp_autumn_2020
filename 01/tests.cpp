#include <iostream>
#include <cassert>

#include "my_allocator.h"
#include "timer.h"

/*
    Some tests :(
*/

int main()
{
    {
        // Stupid tests before calling makeAllocator
        Allocator all;
        assert(all.alloc(1) == nullptr);
        all.reset();
        assert(all.alloc(1) == nullptr);
    }

    {
        // Common tests
        Allocator all;
        all.makeAllocator(100);

        char* carray1 = all.alloc(50);
        assert(carray1 != nullptr);

        char* carray2 = all.alloc(50);
        assert(carray2 != nullptr);

        char* carray3 = all.alloc(2);
        assert(carray3 == nullptr);

        all.reset();
        carray3 = all.alloc(10);
        assert(carray1 == carray3);
    }

    {
        // time test
        Allocator all;
        all.makeAllocator(1'000'000);
        Timer timer;
        size_t i = 0;
        while (++i < 5'000'000) {
            char* tmp = all.alloc(1);
        }
        assert(i == 5'000'000);
        std::cout << "Time for 5'000'000 allocations with Allocator:" << std::endl;
    }

    {
        // Specific test
        Allocator all;
        all.makeAllocator(26);
        char* carray = all.alloc(26);
        for (char c = 'a'; c <= 'z'; ++c) {
            carray[c - 'a'] = c;
        }
        all.reset();
        carray = all.alloc(26);
        for (char c = 'a'; c <= 'z'; ++c) {
            assert(carray[c - 'a'] == c);
        }
    }
    return 0;
}