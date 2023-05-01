#ifndef ARCHETYPE_IDENTIFIER_HPP
#define ARCHETYPE_IDENTIFIER_HPP

/*
* Identifier is used to classify Processor, Entity and
* Archetype.
*/

#include "Macros.hpp"
#include "Properties.hpp"

#include <bitset>
#include <unordered_set>
#include <vector>

namespace ECS
{
    // Represent components list of Processor, Archetype or Entity
    class ARCHETYPE_API Identifier
    {
    public:
        Identifier();
        Identifier(const std::vector<ComponentType>& ts);
        void setType(ComponentType t1);
        void setType(const std::vector<ComponentType>& ts);
        void removeType(ComponentType t);
        // Check if id's component types are all included in this Identifier
        bool contain(const Identifier& id) const;
        const std::bitset<MAX_COMPONENT_TYPE>& getValue() const;
        const std::unordered_set<ComponentType>& getAllType() const;
        // Swap the contents of two Identifiers
        void swap(Identifier& obj);
    private:
        std::bitset<MAX_COMPONENT_TYPE> mValue;
        std::unordered_set<ComponentType> mTypes;
    };
}

#endif // ARCHETYPE_IDENTIFIER_HPP
