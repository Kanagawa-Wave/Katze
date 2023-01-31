#pragma once

#include <glm/gtc/matrix_transform.hpp>

#include "Components.h"
#include "Mesh.h"

class GameObject
{
public:
    using id_t = unsigned int;
    using Map = std::unordered_map<id_t, GameObject>;

    static GameObject CreateGameObject()
    {
        static id_t currentID = 0;
        return GameObject(currentID++);
    }

    static GameObject* CreateGameObjectPtr()
    {
        static id_t currentID = 0;
        return new GameObject(currentID++);
    }

    GameObject() = delete;
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) = default;
    GameObject& operator=(GameObject&&) = default;

    inline id_t GetID() const { return id; }
    inline MeshComponent* GetMesh() { return &mesh; }

    inline void AddMesh(const std::string& meshPath, const std::string& shaderPath, const std::string& texturePath)
    {
        mesh = MeshComponent(meshPath, texturePath);
    }

    inline void AddMesh(const MeshComponent& meshComp) { mesh = meshComp; }

    inline const MeshComponent& GetMesh() const { return mesh; }
    inline const TransformComponent& GetTransform() const { return transform; }

    inline void SetTransform(const glm::vec3& location, const glm::vec3& rotation, const glm::vec3& scale)
    {
        transform = TransformComponent(location, rotation, scale);
    }

    inline void SetLocation(const glm::vec3& location) { transform.Translation = location; }
    inline void Translate(const glm::vec3& translation) { transform.Translation += translation; }
    inline void SetRotation(const glm::vec3& rotation) { transform.Rotation = rotation; }
    inline void SetRotationX(float x) { transform.Rotation.x = x; }
    inline void SetRotationY(float y) { transform.Rotation.y = y; }
    inline void SetRotationZ(float z) { transform.Rotation.z = z; }
    inline void SetScale(const glm::vec3& scale) { transform.Scale = scale; }

private:
    GameObject(id_t objID) : id(objID)
    {
    }

    id_t id;
    MeshComponent mesh;
    TransformComponent transform;
};
