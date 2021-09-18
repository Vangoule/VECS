# VECS
Vangoule's Entity Component System, an ECS using Modern C++ features to allow contingent storage of components with an easy to use interface. This ECS should ensure type safety through C++20 concepts and templated storage design. This ECS was built mostly as a learning exercise, and employs a lot of modern C++ features to create an easy to use interface for a reasonably efficient ECS. 

## What is an ECS and what is the purpose of this ECS?
Wikipedia: https://en.wikipedia.org/wiki/Entity_component_system

For most, the purpose of the Entity Component System is to decouple functionality from data and allow efficient storage of entities data. When data is used, it should also be used as such so instead of calling each entity to do something with a component, the components themselves can be iterated over and used. The development of this ECS has kept this in mind and after researching the available options a static templated storage type was created to ensure all components of a certain type are stored together, this allows any particular system interacting with the data to have less cache misses and therefore theoretically will run faster.

## Basic Usage:
```
struct TransformComponent : public VECS::Component
{
	float x, y, z;

	TransformComponent() : x(0), y(0), z(0) {}
	TransformComponent(float x_in, float y_in, float z_in) : x(x_in), y(y_in), z(z_in) {}
};

struct ScaleComponent : public VECS::Component
{
	float x, y, z;

	ScaleComponent() : x(0), y(0), z(0) {}
	ScaleComponent(float x_in, float y_in, float z_in) : x(x_in), y(y_in), z(z_in) {}
};

int main()
{
	/*Create universe*/
	auto ecs = new VECS::ECS();
	auto universe = ecs->createUniverse();

	/*Add a system*/
	TestSystem system1;
	ecs->getSystemManager()->registerSystem(&system1);

	/*Create an entity*/
	auto ent = universe->createEntity();
	TransformComponent comp1 = TransformComponent(5, 5, 5);
	ScaleComponent comp2 = ScaleComponent(1, 1, 1);
	universe->attachComponent(ent, comp1);
	universe->attachComponent(ent, comp2);

	while (isRunning)
	{
		/*Tick all entity events, and systems.*/
		ecs->tick();
	}
  
        ecs->destroyUniverse();
}
```

## Creating and handling events:
```
/*Create a new event*/
struct MyEvent : public VECS::Event
{
	std::string myData;
};

/*Create an event handler for the default event EntityCreated */
auto eventHandler = m_eventManager->addEventListener<VECS::Events::EntityCreated>([this](auto event) {
			std::cout << "Entity added: " << event.entity << std::endl;
			});

/*Remove an event handler by ID*/
m_eventManager->removeEventListener(eventHandler);

```

## Creating and Adding Systems
```
class TestSystem : public VECS::System
{
public:
  void init()
  {
  
  }
  
  void shutdown()
  {

  }

  void tick()
  {
  
  }
}

/*Adding the system*/
TestSystem system1;
ecs->getSystemManager()->registerSystem(&system1);
  
/*Removing the system*/
ecs->getSystemManager()->removeSystem(&system1);

```

## Accessing entities from a system
There are multiple ways to access entities, the most efficient way would be to keep a local store of components the system is interested in by requesting the list on init from the universe and updating it whenever a ComponentAttached event happens. If you want quick access to components without keeping a store there are several functions available:
```
/*For each transform component, run a function on the component. 
  This is the most efficient method as it simply itterates over each component in a storage*/
m_universe->eachComponent<TransformComponent>([&](auto comp1) {

	});

/*For each entity with a single component run a function. This takes longer as it itterates over each entity,
  then checks if it has the component, gets a handle to the component and then calls the function*/
m_universe->eachEntity<TransformComponent>([&](auto ent, auto comp) {

	});

/*For each entity that has all listed components run a function.*/
m_universe->eachEntity<TransformComponent, ScaleComponent>([&](auto comp1, auto comp2) {

	});

/*For each entity, run the function. This allows the user to manually grab handles for whatever purpose.*/
m_universe->run([&](auto ent) {});
```
