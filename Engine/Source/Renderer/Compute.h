#pragma once

#include "Shader.h"
#include "Texture.h"

class Compute
{
public:
    static void DispatchCompute(const Texture2D* outputTexture, const Shader* computeShader);
    
};
