#ifndef ARCHETYPE_PROCESSOR_HPP
#define ARCHETYPE_PROCESSOR_HPP

/*
* One of the two classes directly used by users
* Processor is the base class for user-defined systems
* Once returned from Engine::registerProcessor<T>(), the
* processor can be used to process components stored in
* the engine
* 
* Only requirement for derived class syntax is their 
* constructors must be in the form:
* Derived::Derived(ECS::Engine& engine)
*   : ECS::Processor(engine)
*   // Do something
* {
*   // Do something
* }
*/

#include "Macros.hpp"
#include "IDGenerator.hpp"
#include "Archetype.hpp"

#include <vector>

namespace ECS
{
    class ARCHETYPE_API Engine;

    // Processor can process engine's components directly once returned from Engine::registerProcessor<T>()
    // Follow this syntax:
    // class Derived : ECS::Processor;
    // void Derived::method()
    // {
    //  for (const auto& arch : myProcessor.getData())
    //  for (Entity e : arch.getEntities())
    //  { 
    //      // do something meaningful...
    //  }
    // }
    // Caution: DO NOT remove/add components while in a range-based for loops
    class ARCHETYPE_API Processor
    {
    public:
        Processor(Engine& engine);
        void setIdentifier(const Identifier& id);
        std::vector<Archetype*>& getData();
    private:
        Engine& mEngine;
        Identifier mID;
        std::vector<Archetype*> mArchetypeRefs;
    };
}

#endif // ARCHETYPE_PROCESSOR_HPP
