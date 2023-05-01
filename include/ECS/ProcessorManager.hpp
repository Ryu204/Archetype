#ifndef ARCHETYPE_PROCESSORMANAGER_HPP
#define ARCHETYPE_PROCESSORMANAGER_HPP

/*
* Wrapper class for Processors' registration
* and Identifier management
*/

#include "Macros.hpp"
#include "Processor.hpp"

namespace ECS
{
    // Wrapper class for Processors' registration and Identifier management
    class ARCHETYPE_API ProcessorManager
    {
    public:
        ProcessorManager(Engine& engine);

        template <typename T>
        std::shared_ptr<T> registerProcessor();

        template <typename T>
        void setIdentifier(const Identifier& id);
    private:
        Engine& mEngine;
        std::unordered_map<const char*,
            std::shared_ptr<Processor>> mProcessors;
    };

    template <typename T>
    std::shared_ptr<T> ProcessorManager::registerProcessor()
    {
        const char* name = typeid(T).name();
        ECS_ASSERT(mProcessors.find(name) == mProcessors.end(),
                   ((std::string)"Processor of type " + (typeid(T).name()) + " registered twice"));
        std::shared_ptr<T> res = std::make_shared<T>(mEngine);
        mProcessors[name] = res;
        return res;
    }

    template <typename T>
    void ProcessorManager::setIdentifier(const Identifier& id)
    {
        const char* name = typeid(T).name();
        ECS_ASSERT(mProcessors.find(name) != mProcessors.end(),
                   ((std::string)"Processor of type " + (typeid(T).name()) + " was not registered"));
        mProcessors[name]->setIdentifier(id);
    }
}

#endif // ARCHETYPE_PROCESSORMANAGER_HPP

