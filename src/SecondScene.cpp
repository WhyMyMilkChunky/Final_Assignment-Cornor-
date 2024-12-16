#include "SecondScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"
#include <imgui/imgui.h>

constexpr int IMAGE_SIZE = 512;
float CubeRot = 0.0f;
Vector3 light2Position = {0.0,0.3,4.0};
void SecondScene::OnLoad()
{
	LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
	LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);
}

void SecondScene::OnUnload()
{
	UnloadTexture(&mTexture);
	UnloadImage(&mImage);
}

void SecondScene::OnUpdate(float dt)
{
	ClearColor(&mImage, MAGENTA);
	ClearDepth(&mImage, 1.0f);

	const float rotationSpeed = 90.0f;
	CubeRot += rotationSpeed * dt;

	Matrix translation = Translate({0.0,0.0,0.0});
	Matrix rotation = RotateY(DEG2RAD);
	Matrix scale = Scale(1, 1, 1);

	Matrix model = scale * rotation * translation;
	Vector3 cameraPos = { 0, 0, 10 };
	Matrix view = LookAt(cameraPos, V3_ZERO, V3_UP);
	Matrix proj = Perspective(45 * DEG2RAD, 1.0f, 0.1, 100);
	Matrix mvp = model * view * proj;

	Light light = CreateLight(light2Position, { 0.3f,0.3f,0.3f }, 0.25, 1.0, 10);

	UniformData uniform;
	uniform.mvp = model * view * proj;
	uniform.cameraPos = cameraPos;
	uniform.light = light;
	uniform.light.position = light2Position;

	uniform.world = model;
	uniform.normal = NormalMatrix(model);

	DrawMesh(&mImage, gMeshHead, uniform, FRACTAL);

	if (CubeRot >= 360.0f)
		CubeRot -= 360.0f;

	if (IsKeyPressed(KEY_4)) {
	Scene:Change(THIRD);
	}
}

void SecondScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Light Position", &light2Position.x, -15, 15);
}

void SecondScene::OnDraw()
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
