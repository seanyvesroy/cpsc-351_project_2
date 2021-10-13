**CPSC 351-04 
Project 2: Implementing the Memory API**

**Group Members**
Clay Golan: clayg@csu.fullerton.edu
Sean McCarthy: spmccarthy4@csu.fullerton.edu
Malka Lazerson: mlazerson@csu.fullerton.edu

**Project Description**
Wel implemented a subset of the POSIX memory allocation API (the malloc() and free() functions) by managing a small block of bytes. Our group dynamically allocated a small array of [std::byte](https://en.cppreference.com/w/cpp/types/byte) objects to manage a block of bytes as if it were a heap. The data member Allocator::heap points to this block of bytes, and Allocator::heap_size stores its size. This heap remains a fixed size throughout the life of the Allocator object. (Though you may, of course, create a new Allocator with a different size). We implemented these methods to allocate memory from the Allocator::heap array using the “first fit” free-space management strategy described in Chapter 17 of the textbook.

**Documentation**
We used a C++ map and C++ List to make our Allocator class. The main parts of the class are functions for malloc(), free(), dump(), and a destructor. We wrote a custom malloc() and free() for this project. 

*malloc()*

Our function takes the size of space we want to allocate for a size of size_t. We find an address with free space, and delete any stray empty nodes. We create two iterators and use nested for loops to compare each element of the free list, and coalesce where possible. If the address in first iterator equals size and address in the other iterator, we will coalesce, determine new values for size and address, and delete old node. If the available space is greater than the space we want to allocate, we set the available space to less the size of what we are allocating. We add the allocated value to list to store for when we want to deallocate, and return original beginning of the free space (now beginning of our allocated space). If the program tries to allocate more space than available, null is returned.  
            
*free()*

Our function starts with a pointer to the address of  allocated space that we want to free. We find the size of what we want to free, and the index on heap where the beginning of what we want to free is located. We find the address of what we want to free, find the size of this space, and free the memory from the allocated list. We also coalesce the same way we do in malloc. We must check in both functions, otherwise the program does not always coalesce everything. We loop through the memory we are freeing in heap, and set all values in heap of memory we deallocated back to their default value. 

**Source files**

-   `allocator.cpp`  (custom memory allocator)
    
-   `allocator.h`  (header file containing class definition)
    
-   `testalloc.cpp`  (client program that uses the allocator)
 
**Testing**

The program  `testalloc`  tests the  `Allocator`  class with the following scenario:

1.  Create an allocator object with the default memory size of 4KB.
    
2.  Create block A of size 1KB, filling it with the letter 'A.'
    
3.  Create block B of size 2KB, filling it with the letter 'B.'
    
4.  Try to create block C of size 2 KB. This will fail since 3KB are already in use.
    
5.  Verify that the contents of blocks A and B are intact, then free Block B
    
6.  Try again to create block C. Since B has been freed, this should succeed.
    
7.  Free blocks A and C.
    
8.  Dump the contents of the allocator's memory.  _Note_: when debugging, you may wish to do this earlier in the program as well.
    
9.  Destroy the allocator object.
    

_Note_: While  `testalloc`  makes for a good sanity-check, we did not rely exclusively on this test. 

**Tarball Contents**
1.  A README file 
2.  The source code for each utility in C++17
3.  A Makefile 

> Written with [StackEdit](https://stackedit.io/).
