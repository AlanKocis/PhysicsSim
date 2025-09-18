#ifndef STATIC_POOL_H
#define STATIC_POOL_H

#include <h/Defaults.h>
#include <vector>

#define INVALID_ID -1

template <typename T>
struct hobPool
{
	T buffer[ENTITY_ALLOC_COUNT]{};
	std::vector<int> free_list;
//
	hobPool();
	void free_all();
	int alloc();
};

template<typename T>
inline hobPool<T>::hobPool()
{
	free_all();
}

template<typename T>
inline void hobPool<T>::free_all()
{
	memset(this->buffer, 0, ENTITY_ALLOC_COUNT * sizeof(T));
	free_list.clear();
	free_list.reserve(ENTITY_ALLOC_COUNT);

	for (int i = 0; i < ENTITY_ALLOC_COUNT; i++)
	{
		free_list.push_back(i);
	}
}

template<typename T>
inline int hobPool<T>::alloc()
{
	if (free_list.empty())
		return INVALID_ID;

	int index = free_list.back();
	free_list.pop_back();
	return index;
}


#endif