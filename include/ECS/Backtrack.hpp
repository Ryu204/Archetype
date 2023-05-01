#ifndef ARCHETYPE_BACKTRACK_HPP
#define ARCHETYPE_BACKTRACK_HPP

#include "Macros.hpp"

namespace ECS
{
    #ifdef ARCHETYPE_DEBUG
    // Print the explanation and collective data in case the library intrusively calls "exit(-1)"
    void ARCHETYPE_API printStackTrace();
    #endif
}

#endif // ARCHETYPE_BACKTRACK_HPP
