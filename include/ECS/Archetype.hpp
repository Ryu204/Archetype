#ifndef ARCHETYPE_ARCHETYPE_HPP
#define ARCHETYPE_ARCHETYPE_HPP

/*
* Archetype consists of ComponentVectors of
* multiple component types. An entity's data
* is stored inside an archetype if its ID is
* the same with that of the archetype
*/

#include "Macros.hpp"
#include "ComponentVector.hpp"
#include "Properties.hpp"
#include "Identifier.hpp"
#include "IDGenerator.hpp"

#include <unordered_map>
#include <memory>

namespace ECS
{
    class ARCHETYPE_API Archetype
    {
    public:
        Archetype();
        Archetype(const Archetype& copyObject);
        Archetype& operator = (Archetype&& obj);
        void clear();

        // Transfer entity's data to new Archetype by only copying valid data
        void transferEntity(Entity entity, Archetype& newArch);

        // Archetype's manipulation itself

        template <typename T>
        void addType(const IDGenerator& generator);
        template <typename T>
        void removeType(const IDGenerator& generator);
        template <typename T>
        bool haveType() const;
        Identifier getIdentifier() const;

        // Entities' data manupulation

        template <typename T>
        T& getComponent(Entity entity);
        template <typename T>
        const T& getComponent(Entity entity) const;
        template <typename T>
        void setComponent(Entity entity, const T&& component);
        template <typename T>
        void setComponent(Entity entity, const T& component);

        // Also keep track of inside entities

        void removeEntity(Entity entity);
        void addEntity(Entity entity);
        const std::unordered_set<Entity>& getEntities() const;
    private:
        template <typename T>
        ComponentVector<T>& getComponentVector();
        template <typename T>
        const ComponentVector<T>& getComponentVector() const;
    private:
        std::unordered_map<const char*, std::shared_ptr<IComponentVector>> mVectors;
        Identifier mID;
        std::unordered_set<Entity> mEntities;
    };

    template <typename T>
    bool Archetype::haveType() const
    {
        return mVectors.find(typeid(T).name()) != mVectors.end();
    }

    template <typename T>
    void Archetype::addType(const IDGenerator& generator)
    {
        ECS_ASSERT(haveType<T>() == false, ((std::string)"Component type " + (typeid(T).name()) + " added twice in archetype"));

        const char* name = typeid(T).name();
        mVectors.emplace(name, std::make_shared<ComponentVector<T>>());
        mID.setType(generator.getType<T>());
    }

    template <typename T>
    void Archetype::removeType(const IDGenerator& generator)
    {
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not added in archetype but query removal"));

        const char* name = typeid(T).name();
        mVectors.erase(name);
        mID.removeType(generator.getType<T>());
    }

    template <typename T>
    ComponentVector<T>& Archetype::getComponentVector()
    {
        return *std::dynamic_pointer_cast<ComponentVector<T>>(mVectors[typeid(T).name()]);
    }

    template <typename T>
    const ComponentVector<T>& Archetype::getComponentVector() const
    {
        const char* name = typeid(T).name();
        ECS_ASSERT(mVectors.find(name) != mVectors.end(), ((std::string)"Component type " + (typeid(T).name()) + " was not added in archetype but query vector reference"));
        return *std::dynamic_pointer_cast<ComponentVector<T>>(mVectors.at(name));
    }

    template <typename T>
    T& Archetype::getComponent(Entity entity)
    {
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not added to archetype but query component data of entity " + std::to_string(entity)));
        return getComponentVector<T>()[entity];
    }

    template <typename T>
    const T& Archetype::getComponent(Entity entity) const
    {
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not added to archetype but query component data of entity " + std::to_string(entity)));
        return getComponentVector<T>()[entity];
    }

    template <typename T>
    void Archetype::setComponent(Entity entity, const T& component)
    {
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not added to archetype but query assignment of entity " + std::to_string(entity)));
        auto& vec = getComponentVector<T>();
        vec.removeData(entity);
        vec.addData(entity, component);
    }

    template <typename T>
    void Archetype::setComponent(Entity entity, const T&& component)
    {
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not added to archetype but query assignment of entity " + std::to_string(entity)));
        auto& vec = getComponentVector<T>();
        vec.removeData(entity);
        vec.addData(entity, component);
    }
}

#endif // ARCHETYPE_ARCHETYPE_HPP
