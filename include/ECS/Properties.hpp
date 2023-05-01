#ifndef ARCHETYPE_PROPERTIES_HPP
#define ARCHETYPE_PROPERTIES_HPP

/*
* Numerical limits of the engine
*/

#include <stdint.h>

namespace ECS
{
    using Entity = uint32_t;
    using ComponentType = uint8_t;
    constexpr Entity MAX_ENTITY = 50000;
    constexpr ComponentType MAX_COMPONENT_TYPE = 50;
    constexpr uint32_t MAX_ARCHETYPE = 200;
}

#endif // ARCHETYPE_PROPERTIES_HPP
