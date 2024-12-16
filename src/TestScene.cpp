#include "TestScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

constexpr int IMAGE_SIZE = 512;

// Global rotation angle
float gCubeRotation = 0.0f;

void TestScene::OnLoad()
{
    
    LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
    LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);
}

void TestScene::OnUnload()
{
    UnloadTexture(&mTexture);
    UnloadImage(&mImage);
}

void TestScene::OnUpdate(float dt)
{
    ClearColor(&mImage, RED);
    ClearDepth(&mImage, 1.0f);

    const float rotationSpeed = 90.0f; 
    gCubeRotation += rotationSpeed * dt;

    Matrix model = Translate({0,0,0}) * RotateY(gCubeRotation * DEG2RAD);
    Matrix view = LookAt({ 0.0f, 0.0f, 5.0f }, V3_ZERO, V3_UP);
    Matrix proj = Perspective(45.0f * DEG2RAD, 1.0f, 0.1f, 100.0f);
    Matrix mvp = model * view * proj;

    DrawMesh(&mImage, gMeshCube, mvp, model);

    if (gCubeRotation >= 360.0f)
        gCubeRotation -= 360.0f;

    if (IsKeyPressed(KEY_3)) {
    Scene:Change(SECOND);
    }
}

void TestScene::OnDraw()
{
 
    UpdateTexture(mTexture, mImage);

    BindTexture(mTexture);
    BindShader(&gShaderFSQ);
    SendInt("u_tex", 0);
    BindFsq();
    DrawFsq();
    UnbindShader();
    UnbindTexture(mTexture);
}
