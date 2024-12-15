#pragma once
#include "Scene.h"
#include "Image.h"
#include "Texture.h"
#include "Math.h"

class FourthScene : public Scene
{
public:
    void OnLoad() final;
    void OnUnload() final;
    void OnUpdate(float dt) final;
    void OnDraw() final;

private:
    Image mImage;
    Texture mTexture;

    static constexpr int GRID_SIZE = 10;
    bool voxels[GRID_SIZE][GRID_SIZE][GRID_SIZE];

    Vector3 cameraPosition = { 0.0f, 2.0f, 5.0f };
    Vector3 cameraFront = { 0.0f, 0.0f, -1.0f };
    Vector3 cameraUp = { 0.0f, 1.0f, 0.0f };
    float cameraYaw = -90.0f; 
    float cameraPitch = 0.0f; 
    float cameraSpeed = 5.0f;
    float sensitivity = 0.1f; 
    Vector2 lastMousePosition = { 0.0f, 0.0f }; 

    void InitializeVoxelGrid();
    void RemoveVoxel(int x, int y, int z);
    bool RaycastVoxel(Vector3 rayOrigin, Vector3 rayDir, int& x, int& y, int& z);
};
