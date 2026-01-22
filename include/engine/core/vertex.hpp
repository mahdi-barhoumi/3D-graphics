#pragma once
#include <engine/core/math.hpp>

namespace Engine
{
    struct VertexP3
    {
        Vector3 position = Vector3(0.0f);
    };
    struct VertexP3C4
    {
        Vector3 position = Vector3(0.0f);
        Vector4 color = Vector4(0.0f);
    };
    struct VertexP3N3
    {
        Vector3 position = Vector3(0.0f);
        Vector3 normal = Vector3(0.0f);
    };
    struct VertexP3N3C4
    {
        Vector3 position = Vector3(0.0f);
        Vector3 normal = Vector3(0.0f);
        Vector4 color = Vector4(0.0f);
    };
    struct VertexP3T2
    {
        Vector3 position = Vector3(0.0f);
        Vector2 textureCoordinate = Vector2(0.0f);
    };
    struct VertexP3T2N3
    {
        Vector3 position = Vector3(0.0f);
        Vector2 textureCoordinate = Vector2(0.0f);
        Vector3 normal = Vector3(0.0f);
    };
}
