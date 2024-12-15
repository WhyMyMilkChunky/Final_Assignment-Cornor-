#include "ThirdScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"
#include <imgui/imgui.h>


//load object and add phong lighting
constexpr int IMAGE_SIZE = 512;

Vector3 objectPosition = {};
Vector3 lightPosition = V3_FORWARD;
Vector3 objectScake = { 1,1,1 };


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
	

	if (IsKeyPressed(KEY_5)) {
	Scene:Change(FOURTH);
	}
}

void ThirdScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Object Position", &objectPosition.x, -8.0f, 8.0f);
	ImGui::SliderFloat3("Light Position", &lightPosition.x, -15.0f, 15.0f);
	ImGui::SliderFloat3("Object Scake", &objectScake.x, 1.0f, 1.0f);
}

void ThirdScene::OnDraw()
{
	UpdateTexture(mTexture, mImage);
	ClearColor(&mImage, BLUE);
	ClearDepth(&mImage, 1);

	float tt = TotalTime();


	Matrix translation = Translate(objectPosition);
	Matrix rotation = RotateY(DEG2RAD);
	Matrix scale = Scale(objectScake);

	Matrix model = scale * rotation * translation;
	Vector3 cameraPos = {0.0f, 0.0f, 10.0f};
	Matrix view = LookAt(cameraPos, V3_ZERO, V3_UP);
	Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, 0.1f, 100.0f);
	Matrix mvp = model * view * proj;

	UniformData uniform;
	uniform.mvp = model * view * proj;
	uniform.cameraPos = cameraPos;
	uniform.light.position = lightPosition;
	uniform.light.diffuse = Vector3({0.5,0.5,0.5 });
	uniform.world = model;
	uniform.normal = NormalMatrix(model);

	DrawMesh(&mImage, gMeshHead,uniform);


	BindTexture(mTexture);
	BindShader(&gShaderPhong);
	SendInt("u_tex", 0);
	BindFsq();
	DrawFsq();
	UnbindShader();
	UnbindTexture(mTexture);
}
