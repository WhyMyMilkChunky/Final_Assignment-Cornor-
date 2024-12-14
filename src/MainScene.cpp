#include "MainScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"
#include <imgui/imgui.h>

constexpr int IMAGE_SIZE = 512;

Vector3 objectPosition = V3_FORWARD * 8.0f;
Vector3 lightPosition = V3_FORWARD * 5.0f;

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

Color ShadePositions(Vector3 position, Vector3 normal)
{
	return Float3ToColor(&position.x);
}

Color ShadeNormals(Vector3 position, Vector3 normal)
{
	return Float3ToColor(&normal.x);
}

void MainScene::OnUpdate(float dt)
{
	ClearColor(&mImage, BLACK);
	ClearDepth(&mImage, 1.0f);

	float tt = TotalTime();
	float near = 0.001f;
	float far = 100.0f;

	Matrix translation = Translate(objectPosition);
	Matrix rotation = RotateY(tt * 00.0f * DEG2RAD);
	Matrix scale = MatrixIdentity();

	Matrix model = scale * rotation * translation;
	Matrix view = LookAt({ 0.0f, 0.0f, 10.0f }, V3_ZERO, V3_UP);
	Matrix proj = Perspective(90.0f * DEG2RAD, 1.0f, near, far);
	 
	UniformData uniform;
	uniform.mvp = model * view * proj;
	uniform.world = model;
	uniform.normal = NormalMatrix(model);

	//uniform.lightPosition = { 0.0f, sinf(tt) * 3.0f, 10.0f};
	uniform.lightPosition = lightPosition;
	uniform.lightColor = { 1.0f, 1.0f, 1.0f };

	DrawMesh(&mImage, gMeshHead, uniform/*, shader*/);
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

// Use this for visual debugging
void MainScene::OnDrawImGui()
{
	ImGui::SliderFloat3("Object Position", &objectPosition.x, -8.0f, 8.0f);
	ImGui::SliderFloat3("Light Position", &lightPosition.x, -15.0f, 15.0f);
}

// Rainbow lighting:
//Vector3 a{ 0.5f, 0.5f, 0.5f };
//Vector3 b{ 0.5f, 0.5f, 0.5f };
//Vector3 c{ 1.0f, 1.0f, 1.0f };
//Vector3 d{ 0.263f, 0.416f, 0.557f };
//uniform.lightColor = Palette(a, b, c, d, cosf(TotalTime()) * 0.5f + 0.5f);

//Shader-switching
//static FragmentShader shader = ShadePositions;
//static bool toggle = false;
//if (IsKeyPressed(KEY_SPACE))
//toggle = !toggle;
//
//shader = toggle ? ShadePositions : ShadeNormals;

// Linearize depth buffer
// Input: depth in non-linear space between [0, 1].
// Output: depth in linear space between [near, far].
//inline float Decode(float depth, float near, float far)
//{
//	depth = depth * 2.0 - 1.0;
//	return (2.0 * near * far) / (far + near - depth * (far - near));
//}
//
//// Input: depth in linear space between [near, far].
//// Output: depth in linear space between [0, 1].
//inline float NormalizeDepth(float depth, float near, float far)
//{
//	return (depth - near) / (far - near);
//}

// Manually test clip-space conversion
//Matrix mvp = view * proj;
//Vector4 clip = { 0.0f, 0.0f, 9.9985f, 1.0f };
//clip = mvp * clip;
//clip /= clip.w;