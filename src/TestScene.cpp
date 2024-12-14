#include "TestScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Shader.h"
#include "imgui/imgui.h"

constexpr int IMAGE_SIZE = 512;

float fov = 90.0f * DEG2RAD;
Matrix mvp = MatrixIdentity();
Matrix world = MatrixIdentity();
Matrix view = LookAt(V3_FORWARD * 10.0f, V3_ZERO, V3_UP);
Matrix proj = Perspective(fov, SCREEN_ASPECT, 0.1f, 100.0f);

// Extra practice -- Either use an element buffer, or copy tcoords to per-vertex from par_shapes
int shaderIndex = 1;
Shader* shaders[] = { &gShaderColor, &gShaderNormals };//, &gShaderTcoords };
Mesh* mesh = nullptr;
Vector3 color = V3_ONE;

void TestScene::OnLoad()
{
	glEnable(GL_DEPTH_TEST);
	mesh = &gMeshSphere;
}

void TestScene::OnUnload()
{
}

void TestScene::OnUpdate(float dt)
{
	float tt = TotalTime();
	world = RotateY(100.0f * DEG2RAD * tt);
	proj = Perspective(fov, SCREEN_ASPECT, 0.1f, 100.0f);
	mvp = world * view * proj;
}

void TestScene::OnDraw()
{
	BindShader(&gShaderFractal);
	SendFloat("u_time", TotalTime());
	SendVec2("u_resolution", { SCREEN_WIDTH, SCREEN_HEIGHT });
	BindFsq();
	DrawFsq();
	UnbindShader();

	BindShader(shaders[shaderIndex]);
	SendVec3("u_color", color);
	SendMat4("u_world", &world);
	SendMat4("u_mvp", &mvp);
	DrawMesh(*mesh);
	UnbindShader();
}

void TestScene::OnDrawImGui()
{
	static bool demo = false;
	if (demo)
	{
		ImGui::ShowDemoWindow();
		return;
	}
	ImGui::SliderAngle("FoV", &fov, 10.0f, 150.0f);
	ImGui::ColorPicker3("Color", &color.x);

	static const char* shaderNames[] =
	{
		"Color",
		"Normals"//, "Tcoords"
	};

	ImGui::Combo("Shaders", &shaderIndex, shaderNames, IM_ARRAYSIZE(shaderNames));
}
