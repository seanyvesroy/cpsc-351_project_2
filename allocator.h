/*
Project 2
Sean McCarthy spmccarthy4@csu.fullerton.edu 
Clay Golan clayg@csu.fullerton.edu
Malka Lazerson mlazerson@csu.fullerton.edu

*/

#ifndef ALLOCATOR_H
#define ALLOCATOR_H
#include <list>
#include <cstddef>
#include <map>
const auto KB = 1024;

class Allocator
{
public:
    static const auto DEFAULT_MEMORY_SIZE = 4 * KB;
    std::list<std::pair<size_t, size_t>> freeList;//first size_t is size second is addrress
    std::map<size_t, size_t> allocatedList;    //fist size_t is beginning address second is end aaddress

    Allocator(size_t size = DEFAULT_MEMORY_SIZE);
    
    ~Allocator();

    std::byte *malloc(size_t);
    void free(std::byte *);

    void dump();

private:
    size_t heap_size;
    std::byte *heap;
};

#endif
