#include "../include/ECS/Archetype.hpp"

namespace ECS
{
    Archetype::Archetype()
    { }

    Archetype::Archetype(const Archetype& copyObject)
    {
        mID = copyObject.mID;
        for (const auto& pr : copyObject.mVectors)
            mVectors[pr.first] = pr.second->createClone();
    }

    Archetype& Archetype::operator = (Archetype&& obj)
    {
        mVectors.swap(obj.mVectors);
        mID.swap(obj.mID);
        mEntities.swap(obj.mEntities);
        return *this;
    }

    void Archetype::transferEntity(Entity entity, Archetype& newArch)
    {
        newArch.addEntity(entity);
        for (auto& pr : mVectors)
            if (newArch.mVectors.find(pr.first) != newArch.mVectors.end())
                pr.second->overwriteData(entity, newArch.mVectors[pr.first]);
        removeEntity(entity);
    }

    Identifier Archetype::getIdentifier() const
    {
        return mID;
    }

    void Archetype::removeEntity(Entity entity)
    {
        for (const auto& p : mVectors)
            p.second->removeEntity(entity);
        mEntities.erase(entity);
    }

    void Archetype::addEntity(Entity entity)
    {
        for (const auto& p : mVectors)
            p.second->addDefaultData(entity);
        mEntities.insert(entity);
    }

    const std::unordered_set<Entity>& Archetype::getEntities() const
    {
        return mEntities;
    }

    void Archetype::clear()
    {
        mID = Identifier();
        mVectors.clear();
        mEntities.clear();
    }
}
