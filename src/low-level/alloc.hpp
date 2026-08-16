#pragma once


struct Chunk {
    Chunk* next
};

class PoolAllocator 
{
public:
    PoolAllocator(size_t chunksPerBlock) : chunksPerBlock(chunksPerBlock) {}
    void *allocate();
    void deallocate();

private:
    //allocates a larger block (pool) for chunks
    Chunk *allocateBlock();
    
    // num chunks per larger block and allocation ptr
    size_t chunksPerBlock;
    Chunk *alloc = nullptr;
};