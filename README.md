# Usage

Entity creation and components manipulation:

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

engine.destroyEntity(entity);
```

## Processor

Processor is used to efficiently iterate and process components data stored inside entity. 

To define a processor, simply do:

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

To retrieve an instance (to be accurate a shared pointer), do the following (remember to also set identifier of the processor):

```cpp
using namespace ECS;
Engine engine;
auto renderer = engine.registerProcessor<RenderSystem>();
engine.registerComponent<Sprite>();
engine.registerComponent<Color>();
// Some more components...

engine.setProcessorIdentifier<RenderSystem, Sprite, Color>();
```

Finally, to iterate and do the processor's jobs, do:

```cpp
class RenderSystem : public ECS::Processor
{
    // ...
}

void RenderSystem::render(RenderWindow& window)
{
    for (auto arch : getData())
    for (auto entity : arch->getEntities())
    {
        auto& sprite = arch->getComponent<Sprite>(entity);
        auto& color = arch->getComponent<Color>(entity);
        
        // Do something meaningful with sprite and color...
    }
}
```

# Install

If the library is built as dynamic library remember to define this macro (best during compilation process):
``` cpp
#define ARCHETYPE_DLL
```

If the libary is meant to be built with debug modest support, define this macro when build and use the library:
```cpp
#define ARCHETYPE_DEBUG
```

**If that is the case, remember to link "Dbghelp" library when building the library**
