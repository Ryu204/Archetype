#include "../include/ECS/Processor.hpp"
#include "../include/ECS/Engine.hpp"

namespace ECS
{
    Processor::Processor(Engine& engine)
        : mEngine(engine)
    { }

    void Processor::setIdentifier(const Identifier& id)
    {
        mID = id;
    }

    std::vector<Archetype*>& Processor::getData()
    {
        if (mEngine.archetypesChanged())
            mArchetypeRefs = mEngine.getArchetypeRefs(mID);
        return mArchetypeRefs;
    }
}
