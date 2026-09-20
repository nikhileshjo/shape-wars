# Entities and their components
Entity is any object in the game. Each of these entities can be attached with a certain component so that they behave in a certain way.\
Note : Though we say we can "attach" any component to an entity, but in reality they're never detached. As soon as you create an entity object, you have created all the possible component objects and hence consume the space required for them on the memory, but for this project, this will not create any visible performance difference.
* Entity : [docs/Entities_and_components.md](../docs/Entities_and_components.md)
* Components: [docs/Components.hpp](../docs/Components.hpp)

# Components
All enities support the below components. Please note that these componetes just hold relevent data required for the entity to work in a certain way, they do not do any work themselves.
* CTransform : This component is responsible for the kinematics an entity. It holds the postion and velocity and the angle of an entity.
* CCollision : this holds the collion radius of the object
* CScore : Keeps track of the score of the player.
* CShape : This holds the shape of the entity. It only makes circles of N number of corners.
* CLifeSpan : Holds the life span of an entity, it has 2 variables, one that holds the maximum health possible, and other is remaining health out of the maximimum possible health.
* CInput : This just translates input integers into variables, so that it becomes easier to handle the inputs of a player.
# Entity
Entiy uses a tuple to hold all components, that's why all the components take space even when you don't use them.
The entity class makes use of get and set functions for book keeping of the entity and you can use add, remove, has and more functions to manipulate components.
# Using Entity class
Components by themselves are useless, as they store related data and not have any functionality, that's way, we only need to know how to use entities.
```
Entity e; // declaration
e.add<CTransform>; // add a component to the entity


// returns a pointer to the required component
// here CTransform
e.get<CTransform>();

// Check if a componenet is added to the entity
// here we check existance of CTransform
e.has<CTransform>(); 

// Remove a component
// This will not delete any values that were previously stored
// Just set it's existance flage to false
// that way, any system (like movement systems for our example)
// will stop taking affect on the entity
e.remove<CTransform>();
```

The above were all the examples of how to use components. Now lets' see how we can do book keeping of the components
```
// returns an integer
// 0 by default
// this is a unique identifier of an entity
// It's just meant to be like that
// there are no checks that gurantee it
e.getId();

// set entity id by passing an integer
e.setId(1234);

// returns a boolean
// true if entity is alive and false if not
// it can be used to trigger destroy
e.isAlive();

// returns a string
// it's helps you catagorise entities
e.getTag();

// you can set the tag by passing a string
e.setTag("sample text");

// you  can destroy an entity at any time
// this will delete anything you created till now
// it goes back to default values
// except the exists flag which is set to false
destroy();

```