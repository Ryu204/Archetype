#ifndef ARCHETYPE_COMPONENTVECTOR_HPP
#define ARCHETYPE_COMPONENTVECTOR_HPP

/* 
* A component vector consists of 2 parts:
* an inner dense vector and a mapping from 
* entities to inner vector's indices.
* Will be used by Archetypes
*/

#include "Macros.hpp"
#include "Properties.hpp"

#include <vector>
#include <unordered_map>
#include <cassert>
#include <memory>
#include <string>

namespace ECS
{
    // Base class for template ComponentVector
    class ARCHETYPE_API IComponentVector
    {
    public:
        IComponentVector();
        virtual ~IComponentVector();
        virtual void removeEntity(Entity entity) = 0;
        virtual void addDefaultData(Entity entity) = 0;
        virtual std::shared_ptr<IComponentVector> createClone() const = 0;
        virtual void overwriteData(Entity entity, std::shared_ptr<IComponentVector> newVec) = 0;
    };

    // A Component vector can store data tightly packed data and allow query by entity ID
    template<typename T>
    class ComponentVector : public IComponentVector
    {
    public:
        ComponentVector() = default;
        // Create an empty clone of itself to support Archetype cloning
        std::shared_ptr<IComponentVector> createClone() const override;
        void removeEntity(Entity entity) override;
        void addDefaultData(Entity entity) override;
        // Overwrite entity's data in another ComponentVector<T>
        void overwriteData(Entity entity, std::shared_ptr<IComponentVector> newVec) override;

        // Data accesses and manipulations

        T& operator [](Entity);
        bool haveData(Entity entity) const;
        void addData(Entity entity, const T& data);
        void addData(Entity entity, const T&& data);
        void removeData(Entity entity);
    private:
        std::unordered_map<Entity, Entity> mInOut;
        std::unordered_map<Entity, Entity> mOutIn;
        std::vector<T> mContainer;
    };

    template <typename T>
    std::shared_ptr<IComponentVector> ComponentVector<T>::createClone() const
    {
        return std::make_shared<ComponentVector<T>>();
    }

    template <typename T>
    void ComponentVector<T>::overwriteData(Entity entity, std::shared_ptr<IComponentVector> newVec)
    {
        ECS_ASSERT(haveData(entity), ((std::string)"No data of entity " + std::to_string(entity) + " in vector of type " + (typeid(T).name()) + " to transfer"));
        std::shared_ptr<ComponentVector<T>> casted = std::dynamic_pointer_cast<ComponentVector<T>>(newVec);
        ECS_ASSERT(casted->haveData(entity), ((std::string)"No place to overwrite entity " + std::to_string(entity) + " to new vector of type ") + (typeid(T).name()));
        casted->removeData(entity);
        casted->addData(entity, mContainer[mOutIn[entity]]);
    }

    template<typename T>
    T& ComponentVector<T>::operator [](Entity entity)
    {
        ECS_ASSERT(haveData(entity), ((std::string)((std::string)"No data of entity " + std::to_string(entity) + " in vector of type " + (typeid(T).name()))));
        return mContainer[mOutIn[entity]];
    }

    template<typename T>
    bool ComponentVector<T>::haveData(Entity entity) const
    {
        return mOutIn.find(entity) != mOutIn.end();
    }

    template<typename T>
    void ComponentVector<T>::addData(Entity entity, const T& data)
    {
        ECS_ASSERT(haveData(entity) == false, ((std::string)"Data of entity " + std::to_string(entity) + " added twice in vector of type " + (std::string)(typeid(T).name())));
        mContainer.push_back(data);
        mOutIn[entity] = mContainer.size() - 1;
        mInOut[mContainer.size() - 1] = entity;
    }

    template<typename T>
    void ComponentVector<T>::addData(Entity entity, const T&& data)
    {
        ECS_ASSERT(haveData(entity) == false, ((std::string)"Data of entity " + std::to_string(entity) + " added twice in vector of type " + (typeid(T).name())));
        mContainer.push_back(data);
        mOutIn[entity] = mContainer.size() - 1;
        mInOut[mContainer.size() - 1] = entity;
    }

    template<typename T>
    void ComponentVector<T>::removeData(Entity entity)
    {
        ECS_ASSERT(haveData(entity), ((std::string)"Data of entity " + std::to_string(entity) + " in vector of type " + (typeid(T).name()) + " not added yet but query removal"));
        Entity rmIndex = mOutIn[entity], sze = mContainer.size(),
               swappedEntity = mInOut[sze - 1];

        mContainer[rmIndex] = mContainer[sze - 1];
        mContainer.pop_back();

        mOutIn[swappedEntity] = rmIndex;
        mInOut[rmIndex] = swappedEntity;

        mInOut.erase(sze - 1);
        mOutIn.erase(entity);
    }

    template<typename T>
    void ComponentVector<T>::removeEntity(Entity entity)
    {
        if (haveData(entity))
            removeData(entity);
    }

    template<typename T>
    void ComponentVector<T>::addDefaultData(Entity entity)
    {
        addData(entity, T());
    }
}

#endif // ARCHETYPE_COMPONENTVECTOR_HPP
