#include "FourthScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"
#include <imgui/imgui.h>

constexpr int IMAGE_SIZE = 512;

Vector3 objectPos = {};
Vector3 lightPos = V3_FORWARD;
Vector3 LightColor = {0.3f,0.3,0.3f};
Light footLight;
void FourthScene::OnLoad()
{
	LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
	LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);
	
}

void FourthScene::OnUnload()
{
	UnloadTexture(&mTexture);
	UnloadImage(&mImage);
}

void FourthScene::OnUpdate(float dt)
{
	ClearColor(&mImage, GREEN);
	ClearDepth(&mImage, 1);
	footLight = CreateLight({ 0,8,0 }, LightColor,0.5f, 0.5f, 10);
	float tt = TotalTime();

	Matrix translation = Translate({ 0.0f,0.0f,0.0f });
	Matrix rotation = RotateY(DEG2RAD);
	Matrix scale = Scale({3,3,3});

	Matrix model = scale * rotation * translation;
	Matrix view = LookAt({ 0.0f, 0.0f, 10.0f }, V3_ZERO, V3_UP);
	Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, 0.01f, 100.0f);
	Matrix mvp = model * view * proj;

	UniformData uniform;
	uniform.mvp = mvp;
	uniform.cameraPos = { 0.0f, 0.0f, 10.0f };
	uniform.light = footLight;
	uniform.light.position = lightPos;
	uniform.world = model;
	uniform.normal = NormalMatrix(model);


	DrawMesh(&mImage, gMeshJP, uniform,SPOT);
	if (IsKeyPressed(KEY_1))
	{
		Scene::Change(MAIN);
	}
}
void FourthScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Light Position", &lightPos.x, -15.0f, 15.0f);
	ImGui::ColorPicker3("Light Color", &LightColor.x);
}

void FourthScene::OnDraw()
{
	UpdateTexture(mTexture, mImage);



	BindTexture(mTexture);
	BindShader(&gShaderFSQ);
	BindFsq();
	DrawFsq();
	UnbindShader();
	UnbindTexture(mTexture);
}



