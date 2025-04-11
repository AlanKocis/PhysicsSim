#include <h/Entity.h>

CubeEntity::CubeEntity()
{
	
}

PlaneEntity::PlaneEntity()
{

}

EntityID EntityManager::id_gen = 0;
EntityID EntityManager::GenEntityID()
{
	return (id_gen++);
}
