#include "../include/ECS/Identifier.hpp"

#include <iostream>
#include <string>

namespace ECS
{
    Identifier::Identifier()
        : mValue()
        , mTypes()
    { }

    Identifier::Identifier(const std::vector<ComponentType>& ts)
    {
        setType(ts);
    }

    void Identifier::setType(ComponentType t1)
    {
        ECS_ASSERT(t1 < MAX_COMPONENT_TYPE, ((std::string)"Identifier::setType parameter out of bounds: t1 = " + std::to_string(t1)));
        ECS_ASSERT(mValue.test(t1) == false, ((std::string)"Component ID " + std::to_string(t1) + " set in identifier already"));

        mValue.set(t1);
        mTypes.insert(t1);
    }

    void Identifier::setType(const std::vector<ComponentType>& ts)
    {
        for (const auto& i : ts)
            setType(i);
    }

    void Identifier::removeType(ComponentType t1)
    {
        mValue.set(t1, false);
        mTypes.erase(t1);
    }

    bool Identifier::contain(const Identifier& id) const
    {
        return (mValue & id.mValue) == id.mValue;
    }

    const std::bitset<MAX_COMPONENT_TYPE>& Identifier::getValue() const
    {
        return mValue;
    }

    const std::unordered_set<ComponentType>& Identifier::getAllType() const
    {
        return mTypes;
    }

    void Identifier::swap(Identifier& obj)
    {
        std::swap(mValue, obj.mValue);
        mTypes.swap(obj.mTypes);
    }
}
