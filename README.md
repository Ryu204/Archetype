Archetype is my implementation of Entity-Component-System (ECS) model as a dynamic-linked library. Nowadays, ECS is barebone of many big games and engines such as Unity3D, Unreal Engine, Minecraft, DeadCells, etc.

# I. Concepts

My ECS system consists of following concept:

* **Entity:** An unique ID that is used to access data from the engine. Ideally, entity is simply an integer.
* **Component:** A plain old datatype that has no constructor or method. Component is understood to be raw data only. Component can be accessed via entity. Every object in the game world is just some components grouped by an entity.
* **System/Processor:** Objects that processes components to create logic of the game. Each processor is only aware of some components and works independently.
* **Component Type:** An unique ID to mark an object type, is simple a number.
* **Component Vector:** Contains data of a specific components. The data is packed tightly in memory and accessed by a mapping from entity to vector index.
* **Archetype:** A set of component vectors of different types. For each set of components of an entity, an archetype is created.


# II. Features

* Create entity and add/remove/modify their components
* Get decent assertion information if an invalid method is called (including backtracing on MSVC)
* Optimize performane by not remove archetypes until really neccessary
* Inlude both debug and release option when built

# III. Usage

This section presents example on this library's usage.

## 1. Entity
To create an entity and assign some components to it, you can reference this snippet:

```cpp
using namespace ECS;

Engine engine;

engine.registerComponent<Health>();
engine.registerComponent<Transform>();
// Register more components...

auto entity = engine.createEntity();
engine.addComponent(entity, Health{ 1.f });
engine.addComponent(entity, Transform { 1.f, 2.f, 4.f });
// ...

// Kill the entity (not neccessary)
engine.destroyEntity(entity);
```

## 2. Processor

Processor is used to efficiently iterate and process components data associated with entity. 

To define a processor, you must inherit from `ECS::Processor` with the following constructor syntax:

```cpp
class DerivedProcessor : public ECS::Processor
{
public:
    DerivedProcessor(ECS::Engine& engine)
        : ECS::Processor(engine)
        // More members...
    {
        // Construction...
    };
    
    // Class definition
}
```

To retrieve an instance of a processor (more specific, a shared pointer), you can use `ECS::Engine::registerProcessor<typename Proc>()` (remember to also set identifier of the processor):

```cpp
using namespace ECS;
Engine engine;
auto renderer = engine.registerProcessor<RenderSystem>();
engine.registerComponent<Sprite>();
engine.registerComponent<Color>();
// Some more components...

engine.setProcessorIdentifier<RenderSystem, Sprite, Color>();
```

Finally, to iterate and do the processor's jobs, you can define any methods you want. To retrieve data of components, you can check this snippet:

```cpp
class RenderSystem : public ECS::Processor
{
    // ...
}

void RenderSystem::render(RenderWindow& window)
{
    // Iterate archetypes associated to this processor
    for (auto arch : getData())
    // Iterate entities of this archetype
    for (auto entity : arch->getEntities())
    {
        // Retrieve components
        auto& sprite = arch->getComponent<Sprite>(entity);
        auto& color = arch->getComponent<Color>(entity);
        
        // Do something meaningful with sprite and color...
    }
}
```

# Install

When building the source code to a dynamic library, remember to define this macro via compiler options:
``` cpp
#define ARCHETYPE_DLL
```

If the libary is meant to be built with debug modest support, define this macro when build *and* use the library:
```cpp
#define ARCHETYPE_DEBUG
```

**If that is the case, remember to link "Dbghelp" on Windows.**

To use the code inside your program, add this line to your source code:

```cpp
#include "ECS/Archetype.hpp"
```
