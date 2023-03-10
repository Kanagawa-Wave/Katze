#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Camera.h"
#include "Mesh.h"
#include "Skybox.h"


struct TagComponent
{
    std::string Tag;

    TagComponent() = default;
    TagComponent(const TagComponent&) = default;

    TagComponent(const std::string& tag)
        : Tag(tag)
    {
    }
};

struct IDComponent
{
    uint32_t ID;

    IDComponent() = default;
    IDComponent(const IDComponent&) = default;

    IDComponent(uint32_t UUID) : ID(UUID)
    {
    }
};

struct TransformComponent
{
    glm::vec3 Translation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Rotation = {0.0f, 0.0f, 0.0f};
    glm::vec3 Scale = {1.0f, 1.0f, 1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;

    TransformComponent(const glm::vec3& translation)
        : Translation(translation)
    {
    }

    TransformComponent(const glm::vec3& translation, const glm::vec3& rotation, const glm::vec3& scale)
        : Translation(translation), Rotation(rotation), Scale(scale)
    {
    }

    glm::mat4 GetTransform() const
    {
        glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

        return glm::translate(glm::mat4(1.0f), Translation)
            * rotation
            * glm::scale(glm::mat4(1.0f), Scale);
    }

    glm::mat4 GetModelMat() const
    {
        const float c3 = glm::cos(Rotation.z);
        const float s3 = glm::sin(Rotation.z);
        const float c2 = glm::cos(Rotation.x);
        const float s2 = glm::sin(Rotation.x);
        const float c1 = glm::cos(Rotation.y);
        const float s1 = glm::sin(Rotation.y);
        return glm::mat4{
            {
                Scale.x * (c1 * c3 + s1 * s2 * s3),
                Scale.x * (c2 * s3),
                Scale.x * (c1 * s2 * s3 - c3 * s1),
                0.0f,
            },
            {
                Scale.y * (c3 * s1 * s2 - c1 * s3),
                Scale.y * (c2 * c3),
                Scale.y * (c1 * c3 * s2 + s1 * s3),
                0.0f,
            },
            {
                Scale.z * (c2 * s1),
                Scale.z * (-s2),
                Scale.z * (c1 * c2),
                0.0f,
            },
            {Translation.x, Translation.y, Translation.z, 1.0f}
        };
    }

    glm::mat4 GetNormalMat() const
    {
        const float c3 = glm::cos(Rotation.z);
        const float s3 = glm::sin(Rotation.z);
        const float c2 = glm::cos(Rotation.x);
        const float s2 = glm::sin(Rotation.x);
        const float c1 = glm::cos(Rotation.y);
        const float s1 = glm::sin(Rotation.y);

        const glm::vec3 inverseScale = 1.0f / Scale;

        return glm::mat3{
            {
                inverseScale.x * (c1 * c3 + s1 * s2 * s3),
                inverseScale.x * (c2 * s3),
                inverseScale.x * (c1 * s2 * s3 - c3 * s1),
            },
            {
                inverseScale.y * (c3 * s1 * s2 - c1 * s3),
                inverseScale.y * (c2 * c3),
                inverseScale.y * (c1 * c3 * s2 + s1 * s3),
            },
            {
                inverseScale.z * (c2 * s1),
                inverseScale.z * (-s2),
                inverseScale.z * (c1 * c2),
            },
        };
    }
};

struct CameraComponent
{
    Camera Camera;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;
};

struct PointLightComponent
{
    PointLightComponent() = default;
    PointLightComponent(const PointLightComponent&) = default;
};

struct MeshComponent
{
    Mesh StaticMesh;

    MeshComponent() = default;

    MeshComponent(const std::string& meshPath, const std::string& texturePath)
    {
        StaticMesh = Mesh(meshPath, "Shaders/BlinnPhone", texturePath);
    }

    MeshComponent(const MeshComponent&) = default;
};

struct SkyBoxComponent
{
    Skybox* SkyBox;

    SkyBoxComponent() = default;
    SkyBoxComponent(const std::string& path)
    {
        SkyBox = new Skybox(path);
    }
    SkyBoxComponent(const SkyBoxComponent&) = default;
};

template <typename... Component>
struct ComponentGroup
{
};

using AllComponents = ComponentGroup<IDComponent, TagComponent, TransformComponent, CameraComponent,
                                     MeshComponent, SkyBoxComponent>;
