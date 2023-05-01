#ifndef ARCHETYPE_ENGINE_HPP
#define ARCHETYPE_ENGINE_HPP

/*
* One of two classes interacting with users
* Engine class manages entities creation and 
* removal, components and processors registration
*/

#include "ProcessorManager.hpp"
#include "Archetype.hpp"
#include "EntityManager.hpp"
#include  "Record.hpp"

namespace ECS
{
    // Coordinate entities creation and removal, components and processors registration
    class ARCHETYPE_API Engine
    {
    public:
        Engine();
        // Entities

        Entity createEntity();
        void destroyEntity(Entity entity);

        // Components

        template <typename T>
        void registerComponent();
        template <typename T>
        T& getComponent(Entity entity);
        template <typename T>
        bool haveComponent(Entity entity);
        template <typename T>
        void removeComponent(Entity entity);
        template <typename T>
        void addComponent(Entity entity, const T&& component);
        template <typename T>
        void addComponent(Entity entity, const T& component);

        // Processors
        
        // Return true if a new archetype was created/removed since last call
        bool archetypesChanged();
        // Return a list of Archetypes that have identifiers matching id
        std::vector<Archetype*> getArchetypeRefs(const Identifier& id);
        template <typename T>
        std::shared_ptr<T> registerProcessor();
        template <typename Proc, typename T1, typename... Ts>
        void setProcessorIdentifier();
    private:
        // Delete archetypes with 0 entities
        void flushEmpty();
    private:
        // Index of empty archetype
        uint32_t mEmptyRow;
        // Component types to number mapping
        IDGenerator mTypeList;
        // See Record.hpp
        Record mTable;

        // Archetype related

        // Contain every archetypes
        std::array<Archetype, MAX_ARCHETYPE> mArchetypes;
        // Keep track of every entity's current archetype
        // [i] = index of the archetype containing i in mArchetypes
        std::array<uint32_t, MAX_ENTITY> mEntityArchetype;
        // [id] = The index of the archetype with identifier matching id
        std::unordered_map<std::bitset<MAX_COMPONENT_TYPE>, uint32_t> mArchetypeIDs;
        bool mArchetypesChanged;

        // Entities

        EntityManager mEntities;

        // Processors
        ProcessorManager mProcessors;
    };

    template <typename T>
    void Engine::registerComponent()
    {
        ECS_ASSERT(mTypeList.haveType<T>() == false, ((std::string)"Component type " + (typeid(T).name()) + " registered twice"));
        mTypeList.registerType<T>();
    }

    template <typename T>
    T& Engine::getComponent(Entity entity)
    {
        ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        Archetype& holder = mArchetypes[mEntityArchetype[entity]];

        ECS_ASSERT(holder.haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not registered but query data of entity " + std::to_string(entity)));
        return holder.getComponent<T>(entity);
    }

    template <typename T>
    bool Engine::haveComponent(Entity entity)
    {
        ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        Archetype& holder = mArchetypes[mEntityArchetype[entity]];
        return holder.haveType<T>();
    }

    template <typename T>
    void Engine::addComponent(Entity entity, const T& component)
    {
        ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        ECS_ASSERT(haveComponent<T>(entity) == false, ((std::string)"Component of type " + (typeid(T).name()) + " added twice to entity " + std::to_string(entity)));

        // Initialize variables
        Archetype& oldArchetype = mArchetypes[mEntityArchetype[entity]];
        Identifier id = oldArchetype.getIdentifier();
        id.setType(mTypeList.getType<T>());
        uint32_t newArchetypeIndex;

        // if the archetype is not already created
        if (mArchetypeIDs.find(id.getValue()) == mArchetypeIDs.end())
        {
            mArchetypesChanged = true;
            if (mTable.isFull())
                flushEmpty();
            // Information update
            newArchetypeIndex = mTable.addRow(id);
            mArchetypeIDs[id.getValue()] = newArchetypeIndex;
            // Clone the old archetype
            mArchetypes[newArchetypeIndex] = std::move(Archetype(oldArchetype));
            mArchetypes[newArchetypeIndex].addType<T>(mTypeList);
        }
        else
            newArchetypeIndex = mArchetypeIDs[id.getValue()];
        // Finally give the entity a new home
        mEntityArchetype[entity] = newArchetypeIndex;
        oldArchetype.transferEntity(entity, mArchetypes[newArchetypeIndex]);

        // The awaiting addition
        mArchetypes[newArchetypeIndex].setComponent<T>(entity, component);
    }

    template <typename T>
    void Engine::addComponent(Entity entity, const T&& component)
    {
        ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        ECS_ASSERT(haveComponent<T>(entity) == false, ((std::string)"Component of type " + (typeid(T).name()) + " added twice to entity " + std::to_string(entity)));

        // Initialize variables
        Archetype& oldArchetype = mArchetypes[mEntityArchetype[entity]];
        Identifier id = oldArchetype.getIdentifier();
        id.setType(mTypeList.getType<T>());
        uint32_t newArchetypeIndex;
        // if the archetype is not already created
        if (mArchetypeIDs.find(id.getValue()) == mArchetypeIDs.end())
        {
            mArchetypesChanged = true;
            if (mTable.isFull())
                flushEmpty();
            // Information update
            newArchetypeIndex = mTable.addRow(id);
            mArchetypeIDs[id.getValue()] = newArchetypeIndex;
            // Clone the old archetype
            mArchetypes[newArchetypeIndex] = std::move(Archetype(oldArchetype));
            mArchetypes[newArchetypeIndex].addType<T>(mTypeList);
        }
        else
            newArchetypeIndex = mArchetypeIDs[id.getValue()];
        // Finally give the entity a new home
        mEntityArchetype[entity] = newArchetypeIndex;
        oldArchetype.transferEntity(entity, mArchetypes[newArchetypeIndex]);

        // The awaiting addition
        mArchetypes[newArchetypeIndex].setComponent<T>(entity, component);
    }

    template <typename T>
    void Engine::removeComponent(Entity entity)
    {
         ECS_ASSERT(mEntities.isAlive(entity), ((std::string)"Entity " + std::to_string(entity) + " was not created yet"));
        ECS_ASSERT(haveComponent<T>(entity), ((std::string)"Component of type " + (typeid(T).name()) + " was not added to entity " + std::to_string(entity)));

        // Initialize variables
        Archetype& oldArchetype = mArchetypes[mEntityArchetype[entity]];
        Identifier id = oldArchetype.getIdentifier();
        id.removeType(mTypeList.getType<T>());
        uint32_t newArchetypeIndex;

        // if the archetype is not already created
        if (mArchetypeIDs.find(id.getValue()) == mArchetypeIDs.end())
        {
            mArchetypesChanged = true;
            // Information update
            newArchetypeIndex = mTable.addRow(id);
            mArchetypeIDs[id.getValue()] = newArchetypeIndex;

            // Clone the old archetype
            mArchetypes[newArchetypeIndex] = std::move(Archetype(oldArchetype));
            mArchetypes[newArchetypeIndex].removeType<T>(mTypeList);
        }
        else
            newArchetypeIndex = mArchetypeIDs[id.getValue()];

        // Finally give the entity a new home
        mEntityArchetype[entity] = newArchetypeIndex;
        oldArchetype.transferEntity(entity, mArchetypes[newArchetypeIndex]);

        // While transferring the component is already truncated
    }

    template <typename T>
    std::shared_ptr<T> Engine::registerProcessor()
    {
        return mProcessors.registerProcessor<T>();
    }

    template <typename Proc, typename T1, typename... Ts>
    void Engine::setProcessorIdentifier()
    {
        Identifier id = mTypeList.generateIdentifier<T1, Ts...>();
        mProcessors.setIdentifier<Proc>(id);
    }
}

#endif // ARCHETYPE_ENGINE_HPP
