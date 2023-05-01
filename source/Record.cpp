#include "../include/ECS/Record.hpp"
#include <iostream>
#include <string>
namespace ECS
{
    Record::Record()
    {
        for (auto& i : mTable)
            i.reset();
        for (uint32_t i = 0; i < MAX_ARCHETYPE; ++i)
        {
            mAvailableRow.push(i);
            mRowList.set(i, false);
        }
    }

    bool Record::isFull() const
    {
        return !mAvailableRow.empty();
    }

    uint32_t Record::addRow(const Identifier& ID)
    {
        ECS_ASSERT(mAvailableRow.empty() == false, "Too much archetypes added to record");
        auto res = mAvailableRow.top();
        mAvailableRow.pop();
        mRowList.set(res, true);
        for (const auto& i : ID.getAllType())
        {
            ECS_ASSERT(i < MAX_COMPONENT_TYPE, ((std::string)"Out of bounds component ID: " + std::to_string(i)));
            mTable[i].set(res, true);
        }
        return res;
    }

    uint32_t Record::addRow(const std::vector<ComponentType>& cs)
    {
        ECS_ASSERT(mAvailableRow.empty() == false, "Too much archetypes added to record");
        auto res = mAvailableRow.top();
        mAvailableRow.pop();
        mRowList.set(res, true);
        for (const auto& i : cs)
        {
            ECS_ASSERT(i < MAX_COMPONENT_TYPE, ((std::string)"Out of bounds component ID: " + std::to_string(i)));
            mTable[i].set(res, true);
        }
        return res;
    }

    void Record::removeRow(uint32_t row, const Identifier& ID)
    {
        ECS_ASSERT(row < MAX_ARCHETYPE, ((std::string)"Invalid row index of table: " + std::to_string(row)));
        ECS_ASSERT(mRowList.test(row), ((std::string)"Row not added yet: " + std::to_string(row)));
        mAvailableRow.push(row);
        mRowList.set(row, false);
        for (const auto& i : ID.getAllType())
        {
            ECS_ASSERT(i < MAX_COMPONENT_TYPE, ((std::string)"Out of bounds component ID: " + std::to_string(i)));
            mTable[i].set(row, false);
        }
    }

    std::vector<uint32_t> Record::getIntersection(const std::unordered_set<ComponentType>& cs) const
    {
        return allSetBit<MAX_ARCHETYPE>(getAndBitset(cs));
    }

    std::bitset<MAX_ARCHETYPE> Record::getAndBitset(const std::unordered_set<ComponentType>& cs) const
    {
        std::bitset<MAX_ARCHETYPE> bs;
        bs.set();
        for (const auto& i : cs)
            bs &= mTable[i];
        return bs;
    }
}
