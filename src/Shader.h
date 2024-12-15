#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include "Lights.h"
#include "Materials.h"
#include "Math.h"

struct Shader
{
	GLuint id;
	std::unordered_map<const char*, GLint> locs;
};

extern Shader gShaderFSQ;
extern Shader gShaderColor;
extern Shader gShaderPhong;
extern Shader gShaderNormals;

void CreateShaders();
void DestroyShaders();

void BindShader(Shader* shader);
void UnbindShader();

void SendFloat(const char* name, float v);
void SendInt(const char* name, int v);
void SendVec2(const char* name, const Vector2& v);
void SendVec3(const char* name, const Vector3& v);
void SendVec4(const char* name, const Vector4& v);
void SendMat4(const char* name, const Matrix* v);
void SendLight(const char* name, const Light* v);
void SendMaterial(const char* name, const Material* v);
