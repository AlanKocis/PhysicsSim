#include <h/Entity.h>

CubeEntity::CubeEntity()
{
	
}

PlaneEntity::PlaneEntity()
{

}

EntityID EntityManager::id_gen = 0;
size_t EntityManager::free_list_count = 0;
std::vector<EntityID> EntityManager::free_list;

EntityManager::EntityManager()
{
	
}
EntityID EntityManager::GenEntityID()
{
	if (free_list.size() == 0)
	{
		EntityID id = id_gen++;
		return id;
	}
	else
	{
		//remove first added ID from free_list count, return it
		EntityID id = free_list[0];
		if (free_list.size() > 1)
		{
			std::swap(free_list[0], free_list[free_list.size() - 1]);
		}
		free_list.pop_back();
		return id;
	}
}

void EntityManager::ResetIDs()
{
	id_gen = 0;
}

void EntityManager::RecycleEntityID(EntityID id)
{
	assert(id_gen >= id);	//early check if index of id is larger than largest index generated so far
	free_list.push_back(id);
}
