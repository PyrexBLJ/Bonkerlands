#include "system/imports.hpp"

#include <sys/memory.h>

void* operator new(size_t size) {
    void* ptr;
    sys_memory_allocate(((0x10000 + 0xFFFF) >> 16) << 16, SYS_MEMORY_PAGE_SIZE_64K, (sys_addr_t*)&ptr);
    return ptr;
}
void* operator new[](size_t size) {
    void* ptr;
    sys_memory_allocate(((0x10000 + 0xFFFF) >> 16) << 16, SYS_MEMORY_PAGE_SIZE_64K, (sys_addr_t*)&ptr);
    return ptr;
}
void* operator new(unsigned int size, unsigned int) {
    void* ptr;
    sys_memory_allocate(((0x10000 + 0xFFFF) >> 16) << 16, SYS_MEMORY_PAGE_SIZE_64K, (sys_addr_t*)&ptr);
    return ptr;
}
void operator delete(void* ptr) { sys_memory_free((sys_addr_t)ptr); }
void operator delete[](void* ptr) { sys_memory_free((sys_addr_t)ptr); }