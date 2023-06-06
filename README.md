# Entity-Component-System (ECS) Implementation

This header file contains an implementation of an Entity-Component-System (ECS) designed to be efficient, modular, and easy to use for game development and similar high-performance applications.

The ECS is split into several namespaces and classes:

### `ecs::`
The main namespace that includes important constants.

#### `ecs::Entity::`

A namespace that includes types and functions related to entities, including their creation, manipulation, and validation.

#### `ecs::Component::`
A namespace that includes types and functions related to components, including their creation and identification.

#### `ecs::ComponentPool::`
A class that manages a pool of components of a particular type. It allows adding and removing components for entities.

#### `ecs::Scene::`
The main class that represents a scene in the ECS. A scene consists of a number of entities, each of which can have any number of components.

#### `ecs::SceneView::`
A class that allows iterating over entities in a scene that have specific component types.

```
Overview Diagram

| Entities (index) | Component 0   | Component 1   | Componen 2    |
|------------------|---------------|---------------|---------------|
| Entity (0)       |      1        |      0        |     0         | <- bitset
| Entity (1)       |      0        |      1        |     1         | <- bitset
| Entity (2)       |      1        |      0        |     1         | <- bitset
|------------------|---------------|---------------|---------------|
|                  |[ComponentPool]|[ComponentPool]|[ComponentPool]|

- Entity[]        = [(index 0), (index 1), (index 2), ... , (index N)]
- ComponentPool[] = [sizeof(Component X) x Entity N]
```

References:

David Colson (2020) [How to make a simple entity-component-system in C++](https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html).
