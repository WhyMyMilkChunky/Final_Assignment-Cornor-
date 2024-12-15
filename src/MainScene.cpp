#include "MainScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

constexpr int IMAGE_SIZE = 512;

Vector3* CopyPositions(Vector3* vertices, size_t vertexCount)
{
    Vector3* positions = new Vector3[vertexCount];
    memcpy(positions, vertices, sizeof(Vector3) * vertexCount);
    return positions;
}

void TransformPositions(Matrix matrix, Vector3* vertices, size_t vertexCount)
{
    for (size_t i = 0; i < vertexCount; i++)
        vertices[i] = matrix * vertices[i];
}

void MainScene::OnLoad()
{
    LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
    LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);
}

void MainScene::OnUnload()
{
    UnloadTexture(&mTexture);
    UnloadImage(&mImage);
}

void MainScene::OnUpdate(float dt)
{
    ClearColor(&mImage, BLACK);
    ClearDepth(&mImage, 1.0f);

    float totalT = TotalTime();

    //every 3 seconds
    float period = 3.0f;
    float t = fmodf(totalT, period);

    //spiral motion
    float angle = t * 2.0f * PI;
    float startRadius = 8.0f;
    float endRadius = 0.0f;
    float radius = startRadius + (endRadius - startRadius) * (t / period);

    //position in a horizontal spiral plane:
    float x = cosf(angle) * radius;
    float y = sinf(angle) * radius;
    float z = 5.0f; //it in front of the camera

    Matrix model = Translate(x, y, z);

    Matrix view = LookAt({ 0.0f, 0.0f, 10.0f }, V3_ZERO, V3_UP);
    Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, 0.001f, 100.0f);
    Matrix mvp = model * view * proj;
    DrawMesh(&mImage, gMeshSphere, mvp, model);
    if (IsKeyPressed(KEY_2)) {
    Scene:Change(TEST);
    }
}

void MainScene::OnDraw()
{
    UpdateTexture(mTexture, mImage);

    BindTexture(mTexture);
    BindShader(&gShaderPhong);
    SendInt("u_tex", 0);
    BindFsq();
    DrawFsq();
    UnbindShader();
    UnbindTexture(mTexture);
}
