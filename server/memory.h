

#ifndef SERVER_MEMORY_H
#define SERVER_MEMORY_H



#include <iostream>
class memory{
public:
    virtual void* allocate(size_t target_size) const = 0;

    virtual void deallocate(void* result) const = 0;

    virtual ~memory(){

    }
};

#endif //SERVER_MEMORY_H
