#include "MainScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"

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

	float tt = TotalTime();
	Matrix model =
		Translate(0.0f, 0.0f, 5.0f + sinf(tt) * 3.0f);

	Matrix view = LookAt({ 0.0f, 0.0f, 10.0f }, V3_ZERO, V3_UP);
	Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, 0.001f, 10.0f);
	Matrix mvp = model * view * proj;
	DrawMesh(&mImage, gMeshSphere, mvp, model);
}

void MainScene::OnDraw()
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
