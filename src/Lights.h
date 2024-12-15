#pragma once
#include "Math.h"

struct Light {
    Vector3 position{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float radius = 1.0f;
    float shininess = 1.0f;
};

inline Light CreateLight(Vector3 position, Vector3 color, float ambient,float specular, float radius)
{
    Light light;
    light.ambient = color * ambient;
    light.diffuse = color;
    light.specular = color * specular;
    light.radius = radius;
    return light;
}