#ifndef MEMORY_POOL_H
#define MEMORY_POOL_H

#include <h/defaults.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <vector>

struct FreeNode
{
	FreeNode *next;
};

template <typename T>
class MemoryPool
{
private:
	unsigned char *buffer;
	FreeNode *free_list;
	size_t chunk_size;
	size_t buffer_len;
	//
	bool is_power_of_two(uintptr_t x);
	uintptr_t align_forward_uintptr(uintptr_t ptr, uintptr_t align);
	size_t align_forward_size(size_t ptr, size_t align);
public:
	void Init(size_t chunk_count);
	MemoryPool<T>();
	~MemoryPool<T>();
	T *AllocateChunk();
	void FreeChunk(T *chunk_ptr);
	void FreeAllChunks();
};

template<typename T>
inline bool MemoryPool<T>::is_power_of_two(uintptr_t x)
{
	return (x & (x - 1)) == 0;
}

template<typename T>
inline uintptr_t MemoryPool<T>::align_forward_uintptr(uintptr_t ptr, uintptr_t align)
{
	uintptr_t a, p, modulo;
	assert(is_power_of_two(align));

	a = align;
	p = ptr;
	modulo = p & (a - 1);
	if (modulo != 0) {
		p += a - modulo;
	}
	return p;
}

template<typename T>
inline size_t MemoryPool<T>::align_forward_size(size_t ptr, size_t align)
{
	size_t a, p, modulo;
	assert(is_power_of_two((uintptr_t)align));

	a = align;
	p = ptr;
	modulo = p & (a - 1);
	if (modulo != 0) {
		p += a - modulo;
	}
	return p;
}

template<typename T>
inline void MemoryPool<T>::Init(size_t chunk_count)
{
	size_t backing_buffer_length = chunk_count * sizeof(T);

	// Store the adjusted parameters
	chunk_size = sizeof(T);
	buffer_len = backing_buffer_length;
	free_list = nullptr;

	this->buffer = new unsigned char[backing_buffer_length];

	// Align backing buffer to the specified chunk alignment
	size_t chunk_alignment = 8;
	uintptr_t initial_start = (uintptr_t)buffer;
	uintptr_t start = align_forward_uintptr(initial_start, (uintptr_t)chunk_alignment);
	backing_buffer_length -= (size_t)(start - initial_start);


	// Align chunk size up to the required chunk_alignment
	chunk_size = align_forward_size(chunk_size, chunk_alignment);

	// Assert that the parameters passed are valid
	assert(chunk_size >= sizeof(FreeNode) &&
		"Chunk size is too small");
	assert(backing_buffer_length >= chunk_size &&
		"Backing buffer length is smaller than the chunk size");

	// Set up the free list for free chunks
	this->FreeAllChunks();
}

template<typename T>
inline MemoryPool<T>::MemoryPool()
{
	buffer = nullptr;
	free_list = nullptr;
	chunk_size = 0;
	buffer_len = 0;
}

template<typename T>
inline MemoryPool<T>::~MemoryPool()
{
	if (buffer)
		delete[] buffer;
}

template<typename T>
inline T *MemoryPool<T>::AllocateChunk()
{
	// Get latest free node
	FreeNode *node = free_list;

	if (!node) {
		assert(0 && "Pool allocator has no free memory");
		return nullptr;
	}

	// Pop free node
	free_list = free_list->next;

	// Zero memory by default
	return (T *)memset((node), 0, chunk_size);
}

template<typename T>
inline void MemoryPool<T>::FreeChunk(T *chunk_ptr)
{
	if (!chunk_ptr)
		return;

	FreeNode *node;

	void *start = buffer;
	void *end = &buffer[buffer_len];

	if (!(start <= chunk_ptr && chunk_ptr < end)) {
		assert(0 && "Memory is out of bounds of the buffer in this pool");
		return;
	}

	// Push free node
	node = (FreeNode *)chunk_ptr;
	node->next = free_list;
	free_list = node;
}

template<typename T>
inline void MemoryPool<T>::FreeAllChunks()
{
	size_t chunk_count = buffer_len / chunk_size;
	size_t i;

	// Set all chunks to be free
	for (i = 0; i < chunk_count; i++) {
		void *ptr = &buffer[i * chunk_size];
		FreeNode *node = (FreeNode *)ptr;
		// Push free node onto the free list
		node->next = free_list;
		free_list = node;
	}
}

#endif