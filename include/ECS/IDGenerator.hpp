#ifndef ARCHETYPE_IDGENERATOR_HPP
#define ARCHETYPE_IDGENERATOR_HPP

/*
* A map from component types T to
* a number type to be used in Identifier
*/

#include "Macros.hpp"
#include "Identifier.hpp"

#include <unordered_map>
#include <typeinfo>
#include <iostream>

namespace ECS
{
    // Generate Identifier based on registered component types
    class ARCHETYPE_API IDGenerator
    {
    public:
        IDGenerator();
        template<typename T>
        void registerType();
        template<typename T1, typename T2, typename... Ts>
        void registerType();
        template <typename T>
        ComponentType getType() const;
        template <typename T>
        Identifier generateIdentifier() const;
        template <typename T1, typename T2, typename... Ts>
        Identifier generateIdentifier() const;
        template <typename T>
        bool haveType() const;
    private:
        std::unordered_map<const char*, ComponentType> mTypeNumber;
        ComponentType mAvailableType;
    };

    template<typename T>
    void IDGenerator::registerType()
    {
        const char* name = typeid(T).name();
        ECS_ASSERT(haveType<T>() == false, ((std::string)"Component type " + (typeid(T).name()) + " registered twice in IDGenerator"));
        ECS_ASSERT(mAvailableType < MAX_COMPONENT_TYPE, ((std::string)"Too much component types registered in IDGenerator"));
        mTypeNumber.emplace(name, mAvailableType++);
    }

    template<typename T1, typename T2, typename... Ts>
    void IDGenerator::registerType()
    {
        registerType<T1>();
        registerType<T2, Ts...>();
    }

    template <typename T>
    ComponentType IDGenerator::getType() const
    {
        auto name = typeid(T).name();
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not registered in IDGenerator"));

        return mTypeNumber.at(name);
    }

    template <typename T>
    Identifier IDGenerator::generateIdentifier() const
    {
        const char* name = typeid(T).name();
        ECS_ASSERT(haveType<T>(), ((std::string)"Component type " + (typeid(T).name()) + " was not registered in IDGenerator"));
        Identifier id;
        id.setType(mTypeNumber.at(name));
        return id;
    }

    template <typename T1, typename T2, typename... Ts>
    Identifier IDGenerator::generateIdentifier() const
    {
        const char* name = typeid(T1).name();
        ECS_ASSERT(haveType<T1>(), ((std::string)"Component type " + (typeid(T1).name()) + " was not registered in IDGenerator"));
        Identifier res = generateIdentifier<T2, Ts...>();
        res.setType(mTypeNumber.at(name));
        return res;
    }

    template <typename T>
    bool IDGenerator::haveType() const
    {
        return mTypeNumber.find(typeid(T).name()) != mTypeNumber.end();
    }
}

#endif // ARCHETYPE_IDGENERATOR_HPP
