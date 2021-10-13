/*
Project 2
Sean McCarthy spmccarthy4@csu.fullerton.edu 
Clay Golan clayg@csu.fullerton.edu
Malka Lazerson mlazerson@csu.fullerton.edu

*/
#include <cassert>

#include <iomanip>
#include <iostream>
#include <iterator>
#include "allocator.h"
#include <list>
#include <unordered_map>

using std::cerr;
using std::endl;

Allocator::Allocator(size_t size)
{
    heap = new std::byte[size]();
    assert(heap != NULL);

    freeList.emplace_front(size, 0);

    this->heap_size = size;
}

Allocator::~Allocator()
{
    delete[] heap;
}

std::byte *Allocator::malloc(size_t size) // size: size of space we want to allocate in bytes
{
    assert(size > 0 && size <= heap_size);

 std::list<std::pair<size_t, size_t>>::iterator iterator; // <size of free space, address>
    for (iterator = freeList.begin(); iterator != freeList.end(); iterator++)
    {
        if (iterator->first == 0) //if we have a freeList node of space zero (empty)
        {
            iterator = freeList.erase(iterator); // delete it
        }
    }

    std::list<std::pair<size_t, size_t>>::iterator iOne; //create two iterators and use nested for loops to compare each element
    std::list<std::pair<size_t, size_t>>::iterator iTwo; //of the free list with each other and coalesce where possible
    for (iOne = freeList.begin(); iOne != freeList.end(); iOne++)
    {
        for (iTwo = freeList.begin(); iTwo != freeList.end(); iTwo++)
        {
            if (iOne->second == ((iTwo->first) + (iTwo->second)))//if address in first iterator equals size and address in the other iterator then coalesce
            {
                iTwo->first = ((iTwo->first) + (iOne->first)); //determine new values for size and address 
                iOne = freeList.erase(iOne);//delete old node
            }
        }
    }
    
    std::list<std::pair<size_t, size_t>>::iterator i;
    for (i = freeList.begin(); i != freeList.end(); i++)
    {
        if (i->first >= size) // if (available space >= size of what we want to allocate)
        {
            i->first = i->first - size;   // set new available space to less the size of what we are allocating
            i->second = i->second + size; // move address of free space up by size of what we are allocating

            allocatedList.insert_or_assign(i->second - size, i->second - 1);//add allocated value to list to store for when we deallocate
                   
            return &(heap[i->second - size]); // return original beginning of the free space (now beginning of our alllocated space)
        }
    }
    return NULL; // this is reached if we try to allocate more than what is available
}
void Allocator::free(std::byte *ptr) // ptr: beginning address of allocated space
{
    assert(ptr != NULL);

    size_t jSize = 0;      // size of what we want to free
    unsigned int addr = 0; // index on heap of beginning of what we want to free

    addr = ptr - heap;//obtains beginning address of the momory we want to free
    jSize = allocatedList.at(addr) + 1 - addr; //determine size of what we want to free
    allocatedList.erase(allocatedList.at(addr));//erase the allocated memory from the allocated list
    
    std::cout << addr << endl;  //beginning address for testing
    std::list<std::pair<size_t, size_t>>::iterator iterator; // <size of free space, address>
    for (iterator = freeList.begin(); iterator != freeList.end(); iterator++)
    {
        if (iterator->first == 0) //if we have a freeList node of space zero (empty)
        {
            iterator = freeList.erase(iterator); // delete it
        }
    }

    freeList.emplace_front(jSize, addr);
   
    std::list<std::pair<size_t, size_t>>::iterator i; //the same way we coalesce in malloc. we need to check in both functions otherwise the program does not alway 
    std::list<std::pair<size_t, size_t>>::iterator iTwo; //coalesce everything. I learned this in debugging.
    for (i = freeList.begin(); i != freeList.end(); i++)
    {
        for (iTwo = freeList.begin(); iTwo != freeList.end(); iTwo++)
        {
            if (i->second == ((iTwo->first) + (iTwo->second)))
            {
                iTwo->first = ((iTwo->first) + (i->first));
                i = freeList.erase(i);
            }
        }
    }

  
    for (unsigned int k = addr; k < jSize + addr; k++) //loop through the memory we are freeing in heap
    {
        heap[k] = (std::byte)'\0'; //setting all values in heap of memory we deallocated back to their default value
    }
    
    
}

void Allocator::dump()
{
    cerr << std::hex << std::uppercase << std::setfill('0');
    for (size_t i = 1; i <= heap_size; i++)
    {
        auto offset = i - 1;
        if (offset % 16 == 0)
        {
            cerr << std::setw(3) << offset << ": ";
        }
        cerr << std::setw(2) << static_cast<unsigned int>(heap[offset]);
        if (i % 16 == 0)
        {
            cerr << endl;
        }
        else
        {
            cerr << ' ';
        }
    }
}