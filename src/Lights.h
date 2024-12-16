#pragma once
#include "Math.h"

struct Light {
    Vector3 position{};
    Vector3 ambient{};
    Vector3 diffuse{};
    Vector3 specular{};
    float radius = 1.0f;
};
enum LightType
{
    POINT,
    SPOT,
    DIRECTIONAL

};

inline Light CreateLight(Vector3 position,Vector3 lightColor, float ambient,float specular, float radius)
{
    Light light;
    light.ambient = lightColor * ambient;
    light.diffuse = lightColor;
    light.specular = lightColor * specular;
    light.radius = radius;
    return light;
}