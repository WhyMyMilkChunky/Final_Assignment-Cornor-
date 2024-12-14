#include "ThirdScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

//load object and add phong lighting
constexpr int IMAGE_SIZE = 512;

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
	ClearColor(&mImage, BLUE);
	DrawMesh(gMeshHead);


	if (IsKeyPressed(KEY_5)) {
	Scene:Change(FOURTH);
	}
}

void ThirdScene::OnDraw()
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
