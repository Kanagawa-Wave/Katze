#include "pch.h"
#include "Compute.h"

#include <glad/glad.h>

void Compute::DispatchCompute(const Texture2D* outputTexture, const Shader* computeShader)
{
    computeShader->Bind();
    outputTexture->Bind();

    glBindImageTexture(0, outputTexture->GetTexture(), 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);

    glDispatchCompute(outputTexture->GetWidth(), outputTexture->GetHeight(), 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}
