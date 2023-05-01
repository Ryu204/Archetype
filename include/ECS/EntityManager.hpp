#ifndef ARCHETYPE_ENTITYMANAGER_HPP
#define ARCHETYPE_ENTITYMANAGER_HPP

/*
* EntityManager manages lifetime of every 
* entities
*/

#include "Macros.hpp"
#include "Properties.hpp"

#include <stack>
#include <bitset>

namespace ECS
{
    // Create and destroy entities
    class ARCHETYPE_API EntityManager
    {
    public:
        EntityManager();
        Entity createEntity();
        void retrieveEntity(Entity e);
        bool isAlive(Entity e) const;
    private:
        std::bitset<MAX_ENTITY> mAlive;
        std::stack<Entity> mContainer;
    };
}

#endif // ARCHETYPE_ENTITYMANAGER_HPP
