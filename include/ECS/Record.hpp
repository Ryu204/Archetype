#ifndef ARCHTYPE_RECORD_HPP
#define ARCHTYPE_RECORD_HPP

/*
    Record Archetype - Component relationship

            C1  C2  C3  ... C50
    A1      ... ... ... ... ...
    A2      ... ... ... ... ...
    A3      ... ... ... ... ...
    ...     ... ... ... ... ...
    A200    ... ... ... ... ...

    When an Archetype is deleted/created, the
    cost of updating the table O(Archetype's types)
    operations.

    Find set of archetype matching a set of Cs?
    Define C(i) = bitset<Column i>
    Set bits of C(i) {Ci in Cs} is the set of archetype needed.

    How to find them fast? Well... iteration...
*/

#include "Macros.hpp"
#include "Properties.hpp"
#include "Identifier.hpp"

#include <bitset>
#include <stack>
#include <vector>
#include <array>
#include <string>

namespace ECS
{
    // Serve the sole purpose of answering this question:
    // "Which archetypes have these component types?"
    class ARCHETYPE_API Record
    {
    public:
        Record();
        bool isFull() const;
        uint32_t addRow(const Identifier& ID);
        uint32_t addRow(const std::vector<ComponentType>& cs);
        void removeRow(uint32_t row, const Identifier& ID);
        std::vector<uint32_t> getIntersection(const std::unordered_set<ComponentType>& cs) const;
    private:
        std::bitset<MAX_ARCHETYPE> getAndBitset(const std::unordered_set<ComponentType>& cs) const;

        // Returns first set bit of a in [start,end) or -1 if all bit is not set
        template <std::size_t N>
        int firstSetBit(const std::bitset<N>& a, int start) const;
        // Returns list of all set bit of bitset a
        template <std::size_t N>
        std::vector<uint32_t> allSetBit(const std::bitset<N>& a) const;
    private:
        std::array<std::bitset<MAX_ARCHETYPE>, MAX_COMPONENT_TYPE> mTable;
        std::stack<uint32_t> mAvailableRow;
        std::bitset<MAX_ARCHETYPE> mRowList;
    };

    template <std::size_t N>
    std::vector<uint32_t> Record::allSetBit(const std::bitset<N>& a) const
    {
        std::size_t cnt = a.count();
        int start = 0;
        std::vector<uint32_t> res;
        while (cnt != 0)
        {
            start = firstSetBit<N>(a, start);
            if (start >= 0 && (int)N > start)
            {
                res.push_back(start);
                ++start;
                --cnt;
            }
            else
                break;
        }
        return res;
    }

    #ifdef __GNUC__
    template <std::size_t N>
    int Record::firstSetBit(const std::bitset<N>& a, int start) const
    {
        ECS_ASSERT(start < (int)N, ((std::string)"Invalid access to bitset \"a\" at index " + std::to_string(start)));
        start = (int)a._Find_next(start - 1);
        if (start == (int)a.size())
            return -1;
        return start;
    }
    #else
    template <std::size_t N>
    int Record::firstSetBit(const std::bitset<N>& a, int start) const
    {
        ECS_ASSERT(start < (int)N, ((std::string)"Invalid access to bitset \"a\" at index " + std::to_string(start)));
        for (int i = start; i < N; ++i)
            if (a.test(i))
                return i;
        return -1;
    }
    #endif // __GNUC__
}
#endif // ARCHTYPE_RECORD_HPP
