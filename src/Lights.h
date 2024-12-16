#pragma once
#include "Math.h"

struct Light {
    Vector3 position{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    Vector3 colour{};
    float radius = 1.0f;
};
enum LightType
{
    POINT,
    SPOT,
    DIRECTIONAL

};

inline Light CreateLight(Vector3 position, float ambient,float specular, float radius)
{
    Light light;
    light.ambient = light.colour * ambient;
    light.diffuse = light.colour;
    light.specular = light.colour * specular;
    light.radius = radius;
    return light;
}