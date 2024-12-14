#include "TestScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"

constexpr int IMAGE_SIZE = 512;

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

Vector3* CopyPositions(Vector3* vertices, size_t vertexCount)
{
	Vector3* positions = new Vector3[vertexCount];
	memcpy(positions, vertices, sizeof(Vector3) * vertexCount);
	return positions;
}

void TestScene::OnUpdate(float dt)
{
	ClearColor(&mImage, BLACK);
	ClearDepth(&mImage, 1.0f);

	float tt = TotalTime();
	ClearColor(&mImage, BLACK);
	const size_t vc = gMeshSphere.vertexCount;
	const size_t fc = gMeshSphere.faceCount;

	Vector3* positions = CopyPositions(gMeshSphere.positions, vc);

	Matrix rotation = RotateY(TotalTime() * 100.0f * DEG2RAD);
	Matrix scale = Scale(0.2, 0.2, 0.2);
	//Matrix translate = Translate(-0.6, sinf(tt) * 0.1 - 0.5 , 0.0);
	Matrix translate = Translate(-0.6, sinf(tt) * 0.4 - 0.5, 0.0);


	for (size_t i = 0; i < fc; i++)
		DrawFaceWireframes(&mImage, positions, i, RandRGB());
	delete[] positions;

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
