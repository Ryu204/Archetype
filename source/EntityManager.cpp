#include "../include/ECS/EntityManager.hpp"

#include <iostream>
#include <vector>
#include <string>

namespace ECS
{
    EntityManager::EntityManager()
        : mContainer()
    {
        for (Entity e = 0; e < MAX_ENTITY; ++e)
        {
            mContainer.push(e);
            mAlive.set(e, false);
        }
    }

    Entity EntityManager::createEntity()
    {
        ECS_ASSERT(mContainer.empty() == false, "Too many entities");
        Entity res = mContainer.top();
        mContainer.pop();
        mAlive.set(res, true);
        return res;
    }

    void EntityManager::retrieveEntity(Entity e)
    {
        ECS_ASSERT(e < MAX_ENTITY, ((std::string)"Entity retrieval got entity " + std::to_string(e) + " out of bounds"));
        ECS_ASSERT(mAlive[e], ((std::string)"Entity " + std::to_string(e) + " was not created yet"));
        mContainer.push(e);
        mAlive.set(e, false);
    }

    bool EntityManager::isAlive(Entity e) const
    {
        ECS_ASSERT(e < MAX_ENTITY, ((std::string)"Entity query of entity " + std::to_string(e) + " out of bound"));
        return mAlive.test(e);
    }
}
