#include "SecondScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

constexpr int IMAGE_SIZE = 512;
float CubeRot = 0.0f;
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

	Matrix model = Translate({ 0,0,0 }) * RotateY(CubeRot * DEG2RAD);
	Matrix view = LookAt({ 0.0f, 0.0f, 5.0f }, V3_ZERO, V3_UP);
	Matrix proj = Perspective(45.0f * DEG2RAD, 1.0f, 0.1f, 100.0f);
	Matrix mvp = model * view * proj;
	Light whiteDirectional = CreateLight({ 0.0,0.0,20.0 }, { 1.0,1.0,1.0 }, 0.25, 0.4, 100);
	DrawMesh(&mImage, gMeshCube, mvp, model, whiteDirectional);

	if (CubeRot >= 360.0f)
		CubeRot -= 360.0f;

	if (IsKeyPressed(KEY_4)) {
	Scene:Change(THIRD);
	}
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
