#pragma once

#include <bitset>
#include <cstdint>
#include <vector>

//=========================
// ECS
//=========================
namespace ecs {

// Define the maximum number of components that an entity can have.
const uint64_t MAX_COMPONENTS = 64;
// Define the maximum number of entities that a scene can register.
const uint64_t MAX_ENTITIES   = 1000000;

// Set bits for components that each entity has.
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

//=========================
// Entity
//=========================
namespace Entity {

// Entity ID = 64bits (32bits for EntityID + 32bits for Entity Version)
typedef uint64_t Id;
typedef uint32_t Index;
typedef uint32_t Version;

/**
 * Create and return a new `EntityId` from an `index` and a `version`.
 */
inline static Id NewId(Index index, Version version) {
    return ((Id)index << 32) | ((Id) version);
}

/**
 * Return the top half index part of the `EntityId`.
 */
inline static Index GetIndex(Id id) {
    return id >> 32;
}

/**
 * Return the below half index part of the `EntityId`.
 */
inline static Version GetVersion(Id id) {
    return (Version)id;
}

/**
 * Invalid if the `id` is uint32_t max (destroied).
 */
inline bool IsValid(Id id) {
    return (id >> 32) != Index(-1);
}
}  // Entity

//=========================
// Component
//=========================
namespace Component {

// Component ID
typedef uint64_t Id;

/**
 * Create and return a new component ID
 * Each Component class has its counter incremented.
 */
inline static Id NewId() {
    static Id value = 0;
    return value++;
}

template<class T>
inline static Id GetId() {
    static Id counter = NewId();
    return counter;
}
}  // Component

//=========================
// ComponentPool
//=========================
class ComponentPool {
public:
    /**
     * Construct a new Component Pool object
     */
    ComponentPool(uint64_t size) : size_(size) {
        head_ = new char[size * MAX_ENTITIES];
    }

    /**
     * Destroy the Component Pool object
     */
    ~ComponentPool() {
        delete[]  head_;
    }

    /**
     * Return a pointer to the component data stored at the index in the pool.
     */
    inline void *Get(Entity::Index index) const {
        return head_ + index * size_;
    }

private:
    uint64_t size_ = 0;
    void    *head_ = nullptr;
};

//=========================
// Scene
//=========================
class Scene {
public:
    /**
     *
     */
    Entity::Id NewEntity() {
        if (!freelist_.empty()) {
            auto i  = freelist_.back(); freelist_.pop_back();
            auto id = Entity::NewId(i, Entity::GetVersion(entities_[i].id_));
            entities_[i].id_ = id;
            return id;
        }
        auto id = Entity::NewId(Entity::Index(entities_.size()), 0);
        entities_.push_back({
            id,
            ComponentMask()
        });
        return id;
    }

    /**
     *
     */
    void RemoveEntity(Entity::Id id) {
        auto i = Entity::GetIndex(id);
        entities_[i].id_ = Entity::NewId(
            Entity::Index(-1),         // Mark as invalid
            Entity::GetVersion(id) + 1 // Increment the version
        );
        entities_[i].mask_.reset();
        freelist_.push_back(i);
    }

    /**
     *
     */
    template<class T>
    bool HasComponent(Entity::Id id) {
        auto eid = Entity::GetIndex(id);
        auto cid = Component::GetId<T>();
        return entities_[eid].mask_.test(cid);
    }

    /**
     *
     */
    template<class T, typename... Args>
    T *AddComponent(Entity::Id id, Args&&... args) {
        auto i = Entity::GetIndex(id);
        auto cid = Component::GetID<T>();
        if (pools_.size() <= cid) {
            pools_.resize(cid + 1, nullptr);
        }
        if (pools_[cid] == nullptr) {
            pools_[cid] = new ComponentPool(sizeof(T));
        }
        auto component = new (pools_[cid]->Get(i)) T(std::forward<Args>(args)...);
        entities_[i].mask.set(cid);
        return component;
    }

    /**
     *
     */
    template<class T>
    void RemoveComponent(Entity::Id id) {
        auto i = Entity::GetIndex(id);
        auto cid = Component::GetId<T>();
        if (entities_[i].id_ != id) {
            return;
        }
        entities_[i].mask_.reset(cid);
    }

    /**
     *
     */
    template<class T>
    T *GetComponent(Entity::Id id) {
        auto i Entity::GetIndex(id);
        auto cid = Component::GetId<T>();
        if (entities_[i].id_ != id) {
            return;
        }
        return (T *)(pools_[cid]->Get(i));
    }

private:
    struct EntityPack {
        Entity::Id    id_;
        ComponentMask mask_;
    };

    std::vector<EntityPack>      entities_;
    std::vector<Entity::Index>   freelist_;
    std::vector<ComponentPool *> pools_;
};

}  // ecs
