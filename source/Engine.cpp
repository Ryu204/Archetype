#include "../include/ECS/Engine.hpp"

namespace ECS
{
    Engine::Engine()
        : mArchetypesChanged(false)
        , mProcessors(*this)
    {
        // Reserve first archetype for empty entity
        mEmptyRow = mTable.addRow(Identifier());
        mArchetypeIDs[std::bitset<MAX_COMPONENT_TYPE>()] = mEmptyRow;
        for (Entity e = 0; e < MAX_ENTITY; ++e)
            mEntityArchetype[e] = mEmptyRow;
        mArchetypes[mEmptyRow] = Archetype();
    }

    Entity Engine::createEntity()
    {
        Entity res = mEntities.createEntity();
        mEntityArchetype[res] = mEmptyRow;
        return res;
    }

    void Engine::destroyEntity(Entity entity)
    {
        // destroy
        ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        mEntities.retrieveEntity(entity);
        for (auto& pr : mArchetypeIDs)
            mArchetypes[pr.second].removeEntity(entity);
    }

    bool Engine::archetypesChanged()
    {
        if (mArchetypesChanged)
        {
            mArchetypesChanged = false;
            return true;
        }
        return true;
    }

    std::vector<Archetype*> Engine::getArchetypeRefs(const Identifier& id)
    {
        std::vector<Archetype*> res;
        for (auto i : mTable.getIntersection(id.getAllType()))
            res.push_back(&mArchetypes[i]);
        return res;
    }

    void Engine::flushEmpty()
    {
        std::vector<std::bitset<MAX_COMPONENT_TYPE>> tobeRemoved;

        for (auto& pr : mArchetypeIDs)
        {
            if (pr.first == std::bitset<MAX_COMPONENT_TYPE>())
                continue;
            if (mArchetypes[pr.second].getEntities().empty())
            {
                mTable.removeRow(pr.second, mArchetypes[pr.second].getIdentifier());
                tobeRemoved.push_back(pr.first);
            }
        }

        for(auto& i : tobeRemoved)
            mArchetypeIDs.erase(i);
    }
}
