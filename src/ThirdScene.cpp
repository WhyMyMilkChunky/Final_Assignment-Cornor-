#include "ThirdScene.h"
#include "Shader.h"
#include "App.h"
#include "Rasterization.h"
#include "Materials.h"
#include "ImageUtils.h"
#include "Window.h"
#include <imgui/imgui.h>


//load object and add phong lighting
constexpr int IMAGE_SIZE = 512;

Vector3 objectPosition = {};
Vector3 lightPosition = V3_FORWARD;


void ThirdScene::OnLoad()
{
	LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
	LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);
	
}

void ThirdScene::OnUnload()
{
	UnloadTexture(&mTexture);
	UnloadImage(&mImage);
}

void ThirdScene::OnUpdate(float dt)
{
    ClearColor(&mImage, BLACK);
    ClearDepth(&mImage, 1);

    float tt = TotalTime();

    float amplitude = 10;
    float frequency = 0.5; //speed
    lightPosition.y = amplitude * sinf(2.0f * PI * frequency * tt);
    lightPosition.z = 5;
    lightPosition.x = 0;

    Matrix translation = Translate(objectPosition);
    Matrix rotation = RotateY(DEG2RAD);
    Matrix scale = Scale(1,1,1);

    Matrix model = scale * rotation * translation;
    Vector3 cameraPos = { 0, 0, 10};
    Matrix view = LookAt(cameraPos, V3_ZERO, V3_UP);
    Matrix proj = Perspective(45 * DEG2RAD, 1.0f, 0.1, 100);
    Matrix mvp = model * view * proj;

    Light light = CreateLight(lightPosition, { 0.3f,0.3f,0.3f }, 0.25, 1.0, 10);

    UniformData uniform;
    uniform.mvp = model * view * proj;
    uniform.cameraPos = cameraPos;
    uniform.light = light;
   // uniform.light.position = lightPosition;

    uniform.world = model;
    uniform.normal = NormalMatrix(model);

    DrawMesh(&mImage, gMeshHead, uniform,POINT);

    if (IsKeyPressed(KEY_5)) {
        Scene::Change(FOURTH);
    }
}


void ThirdScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Object Position", &objectPosition.x, -8, 8);
	ImGui::SliderFloat3("Light Position", &lightPosition.x, -15, 15);
}

void ThirdScene::OnDraw()
{
	UpdateTexture(mTexture, mImage);

    //I wrote phong into the draw mesh  ¯\_( ° > °)_/¯
	BindTexture(mTexture);
	BindShader(&gShaderFSQ);
	SendInt("u_tex", 0);
	BindFsq();
	DrawFsq();
	UnbindShader();
	UnbindTexture(mTexture);
}
