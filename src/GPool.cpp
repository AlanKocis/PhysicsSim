#ifndef G_POOL_HPP
#define G_POOL_HPP

#include <stdint.h>


struct FreeNode
{
	FreeNode *next;
};



template <typename T> class GPool
{
private:
	unsigned char *bytes;
	uint32_t byte_count;





};


#endif