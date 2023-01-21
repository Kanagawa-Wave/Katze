#pragma once
#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

#include "Texture.h"
#include "Scene/Scene.h"

class Renderer
{
public:
    static void Init();
    static void SetClearColor(const glm::vec4& color);
    static void Clear();
    static void ResizeViewport(unsigned width, unsigned height);
    static void Submit(Scene& scene);
};
