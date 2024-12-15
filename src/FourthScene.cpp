#include "FourthScene.h"
#include "Shader.h"
#include "Mesh.h"
#include "App.h"
#include "Rasterization.h"
#include "ImageUtils.h"
#include "Window.h"

constexpr int IMAGE_SIZE = 512;
void FourthScene::InitializeVoxelGrid()
{
	for (int x = 0; x < GRID_SIZE; ++x)
	{
		for (int y = 0; y < GRID_SIZE; ++y)
		{
			for (int z = 0; z < GRID_SIZE; ++z)
			{
				voxels[x][y][z] = (y == 0); 
			}
		}
	}
}

void FourthScene::OnLoad()
{
	LoadImage(&mImage, IMAGE_SIZE, IMAGE_SIZE);
	LoadTexture(&mTexture, IMAGE_SIZE, IMAGE_SIZE);

	InitializeVoxelGrid();
}


void FourthScene::OnUnload()
{
	UnloadTexture(&mTexture);
	UnloadImage(&mImage);
}

bool FourthScene::RaycastVoxel(Vector3 rayOrigin, Vector3 rayDir, int& x, int& y, int& z)
{
	for (float t = 0.0f; t < 100.0f; t += 0.1f)
	{
		Vector3 pos = rayOrigin + rayDir * t;
		int ix = (int)floor(pos.x);
		int iy = (int)floor(pos.y);
		int iz = (int)floor(pos.z);

		if (ix >= 0 && ix < GRID_SIZE && iy >= 0 && iy < GRID_SIZE && iz >= 0 && iz < GRID_SIZE)
		{
			if (voxels[ix][iy][iz])
			{
				x = ix; y = iy; z = iz;
				return true;
			}
		}
	}
	return false;
}
void FourthScene::RemoveVoxel(int x, int y, int z)
{
	if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && z >= 0 && z < GRID_SIZE)
	{
		voxels[x][y][z] = false;
	}
}

void FourthScene::OnUpdate(float dt)
{
	ClearColor(&mImage, GREEN);

	Vector3 movement = { 0.0f, 0.0f, 0.0f };
	if (IsKeyDown(KEY_W)) movement += cameraFront;
	if (IsKeyDown(KEY_S)) movement -= cameraFront;
	if (IsKeyDown(KEY_A)) movement -= Normalize(Cross(cameraFront, cameraUp));
	if (IsKeyDown(KEY_D)) movement += Normalize(Cross(cameraFront, cameraUp));

	if (Length(movement) > 0.0f)
		cameraPosition += Normalize(movement) * cameraSpeed * dt;

	Vector2 mousePos = MousePosition();
	Vector2 mouseDelta = mousePos - lastMousePosition;
	lastMousePosition = mousePos;

	cameraYaw += mouseDelta.x * sensitivity;
	cameraPitch -= mouseDelta.y * sensitivity;

	if (cameraPitch > 89.0f) cameraPitch = 89.0f;
	if (cameraPitch < -89.0f) cameraPitch = -89.0f;


	Vector3 front;
	front.x = cosf(DEG2RAD * cameraYaw) * cosf(DEG2RAD * cameraPitch);
	front.y = sinf(DEG2RAD * cameraPitch);
	front.z = sinf(DEG2RAD * cameraYaw) * cosf(DEG2RAD * cameraPitch);
	cameraFront = Normalize(front);

	if (IsMouseClicked(MOUSE_BUTTON_1))
	{
		Vector3 rayDir = cameraFront; 
		int x, y, z;
		if (RaycastVoxel(cameraPosition, rayDir, x, y, z))
		{
			RemoveVoxel(x, y, z);
		}
	}

	if (IsKeyPressed(KEY_1))
	{
		Scene::Change(MAIN);
	}
}


void FourthScene::OnDraw()
{
	ClearColor(&mImage, GREEN);
	ClearDepth(&mImage, 1.0f);

	Matrix view = LookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	Matrix proj = Perspective(45.0f * DEG2RAD, 1.0f, 0.1f, 100.0f);

	for (int x = 0; x < GRID_SIZE; ++x)
	{
		for (int y = 0; y < GRID_SIZE; ++y)
		{
			for (int z = 0; z < GRID_SIZE; ++z)
			{
				if (voxels[x][y][z])
				{
					Matrix model = Translate({ (float)x, (float)y, (float)z });
					Matrix mvp = model * view * proj;
					DrawMesh(&mImage, gMeshCube, mvp, model);
				}
			}
		}
	}

	UpdateTexture(mTexture, mImage);

	BindTexture(mTexture);
	BindShader(&gShaderFSQ);
	SendInt("u_tex", 0);
	BindFsq();
	DrawFsq();
	UnbindShader();
	UnbindTexture(mTexture);
}


