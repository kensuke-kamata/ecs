#include <iostream>

#include "ECS.h"

struct Position {
    float x;
    float y;
};

struct Rotation {
    float angle;
};

void PhysicsSystem(ecs::Scene &scene, float dt) {
    for (auto id : ecs::SceneView<Position, Rotation>(scene)) {
        // do something...
        std::cout << "PhysicsSystem ID: " << ecs::Entity::GetIndex(id) << std::endl;
    }
}

int main(int, char**) {
    ecs::Scene scene;
    ecs::Entity::Id e1 = scene.NewEntity();
    ecs::Entity::Id e2 = scene.NewEntity();
    ecs::Entity::Id e3 = scene.NewEntity();
    scene.AddComponent<Position>(e1);
    scene.AddComponent<Position>(e2);
    scene.AddComponent<Rotation>(e2);
    scene.AddComponent<Rotation>(e3);

    PhysicsSystem(scene, 0.16f);
}
