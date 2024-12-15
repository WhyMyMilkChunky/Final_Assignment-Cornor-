#include "FourthScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

constexpr int IMAGE_SIZE = 512;

Vector3 objectPos = {};
Vector3 lightPos = V3_FORWARD;
Vector3 objectScale = { 1,1,1 };
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

	if (IsKeyPressed(KEY_1))
	{
		Scene::Change(MAIN);
	}
}
void FourthScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Object Position", &objectPos.x, -8.0f, 8.0f);
	ImGui::SliderFloat3("Light Position", &lightPos.x, -15.0f, 15.0f);
	ImGui::SliderFloat3("Object Scake", &objectScale.x, 1.0f, 1.0f);
}

void FourthScene::OnDraw()
{
	UpdateTexture(mTexture, mImage);
	ClearColor(&mImage, GREEN);
	ClearDepth(&mImage, 1);

	float tt = TotalTime();


	Matrix translation = Translate(objectPos);
	Matrix rotation = RotateY(DEG2RAD);
	Matrix scale = Scale(objectPos);

	Matrix model = scale * rotation * translation;
	Matrix view = LookAt({ 0.0f, 0.0f, 10.0f }, V3_ZERO, V3_UP);
	Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, 0.1f, 100.0f);
	Matrix mvp = model * view * proj;

	DrawMesh(&mImage, gMeshJP, mvp, model);


	BindTexture(mTexture);
	BindShader(&gShaderFSQ);
	SendInt("u_tex", 0);
	BindFsq();
	DrawFsq();
	UnbindShader();
	UnbindTexture(mTexture);
}



