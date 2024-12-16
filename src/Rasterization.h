#pragma once
#include "Image.h"
#include "Mesh.h"
#include "Lights.h"

struct Rect
{
	int xMin;
	int xMax;
	int yMin;
	int yMax;
};

inline void DrawLineX(Image* image, int row, int x0, int x1, Color color)
{
	for (int x = x0; x <= x1; x++)
		SetPixel(image, x, row, color);
}

inline void DrawLineY(Image* image, int col, int y0, int y1, Color color)
{
	for (int y = y0; y <= y1; y++)
		SetPixel(image, col, y, color);
}

inline void DrawLine(Image* image, int x0, int y0, int x1, int y1, Color color)
{
	int dx = x1 - x0;
	int dy = y1 - y0;

	int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
	float xStep = dx / (float)steps;
	float yStep = dy / (float)steps;

	float x = x0;
	float y = y0;
	for (int i = 0; i < steps; i++)
	{
		x += xStep;
		y += yStep;
		SetPixel(image, x, y, color);
	}
}

inline void DrawRect(Image* image, int x, int y, int w, int h, Color color)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			SetPixel(image, x + j, y + i, color);
		}
	}
}

inline void DrawRectLines(Image* image, int x, int y, int w, int h, Color color)
{
	DrawLineX(image, y + 0, x, x + w, color);
	DrawLineX(image, y + h, x, x + w, color);
	DrawLineY(image, x + 0, y, y + h, color);
	DrawLineY(image, x + w, y, y + h, color);
}

inline void DrawCircle(Image* image, int cx, int cy, int cr, Color color)
{
	int x = 0;
	int y = cr;
	int d = 3 - 2 * cr;

	auto line = [&](int lx, int ly, int l)
		{
			for (int dx = -l; dx <= l; dx++)
				SetPixel(image, lx + dx, ly, color);
		};

	while (y >= x)
	{
		line(cx, cy + y, x);
		line(cx, cy - y, x);
		line(cx, cy + x, y);
		line(cx, cy - x, y);

		x++;
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
	}
}

inline void DrawCircleLines(Image* image, int cx, int cy, int cr, Color color)
{
	int x = 0;
	int y = cr;
	int d = 3 - 2 * cr;

	while (y >= x)
	{
		SetPixel(image, cx + x, cy + y, color);
		SetPixel(image, cx - x, cy + y, color);
		SetPixel(image, cx + x, cy - y, color);
		SetPixel(image, cx - x, cy - y, color);
		SetPixel(image, cx + y, cy + x, color);
		SetPixel(image, cx - y, cy + x, color);
		SetPixel(image, cx + y, cy - x, color);
		SetPixel(image, cx - y, cy - x, color);

		x++;
		if (d > 0)
		{
			y--;
			d = d + 4 * (x - y) + 10;
		}
		else
		{
			d = d + 4 * x + 6;
		}
	}
}

inline void DrawFaceWireframes(Image* image, Vector3* positions, size_t face, Color color = WHITE)
{
	size_t vertex = face * 3;
	for (size_t i = 0; i < 3; i++)
	{
		Vector3 v0 = positions[vertex + i];
		Vector3 v1 = positions[vertex + ((i + 1) % 3)];
		v0.x = Remap(v0.x, -1.0f, 1.0f, 0, image->width - 1);
		v0.y = Remap(v0.y, -1.0f, 1.0f, 0, image->height - 1);
		v1.x = Remap(v1.x, -1.0f, 1.0f, 0, image->width - 1);
		v1.y = Remap(v1.y, -1.0f, 1.0f, 0, image->height - 1);

		int x0 = v0.x;
		int y0 = v0.y;
		int x1 = v1.x;
		int y1 = v1.y;
		DrawLine(image, x0, y0, x1, y1, color);
	}
}

inline void DrawMesh(Image* image, Mesh mesh, Matrix mvp, Matrix world)
{
	// screen-space
	Vector3* vertices = new Vector3[mesh.vertexCount];

	// world-space
	Vector3* positions = new Vector3[mesh.vertexCount];
	Vector3* normals = new Vector3[mesh.vertexCount];

	// Extract normal-matrix (remove translation and handle non-uniform scale)
	Matrix normal = world;
	normal.m12 = normal.m13 = normal.m14 = 0.0f;
	normal = Transpose(Invert(normal));

	// Convert mesh vertices from view-space to screen-space
	for (size_t i = 0; i < mesh.vertexCount; i++)
	{
		Vector4 clip;
		clip.x = mesh.positions[i].x;
		clip.y = mesh.positions[i].y;
		clip.z = mesh.positions[i].z;
		clip.w = 1.0f;

		clip = mvp * clip;
		clip /= clip.w;

		Vector3 screen;
		screen.x = Remap(clip.x, -1.0f, 1.0f, 0.0f, image->width - 1.0f);
		screen.y = Remap(clip.y, -1.0f, 1.0f, 0.0f, image->height - 1.0f);
		screen.z = clip.z;

		vertices[i] = screen;
		positions[i] = world * mesh.positions[i];
		normals[i] = Normalize(normal * mesh.normals[i]);
	}

	// Triangle AABBs
	Rect* rects = new Rect[mesh.faceCount];				
	for (size_t face = 0; face < mesh.faceCount; face++)
	{
		// Ensure min & max get overwritten
		int xMin = image->width - 1;
		int yMin = image->height - 1;
		int xMax = 0;
		int yMax = 0;

		// Determine min & max of face, ensure its on-screen
		const size_t vertex = face * 3;
		for (size_t i = 0; i < 3; i++)
		{
			int x = vertices[vertex + i].x;
			int y = vertices[vertex + i].y;
			xMin = std::max(0, std::min(xMin, x));
			yMin = std::max(0, std::min(yMin, y));
			xMax = std::min(image->width - 1, std::max(xMax, x));
			yMax = std::min(image->height - 1, std::max(yMax, y));
		}

		rects[face].xMin = xMin;
		rects[face].xMax = xMax;
		rects[face].yMin = yMin;
		rects[face].yMax = yMax;
	}

	for (size_t face = 0; face < mesh.faceCount; face++)
	{
		for (int x = rects[face].xMin; x <= rects[face].xMax; x++)
		{
			for (int y = rects[face].yMin; y <= rects[face].yMax; y++)
			{
				size_t vertex = face * 3;
				Vector3 v0 = vertices[vertex + 0];
				Vector3 v1 = vertices[vertex + 1];
				Vector3 v2 = vertices[vertex + 2];
				
				// Tri-linear interpolation, ensure 0.0 >= uvw <= 1.0
				Vector3 bc = Barycenter({ (float)x, (float)y, 0.0f }, v0, v1, v2);
				bool low = bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f;
				bool high = bc.x > 1.0f || bc.y > 1.0f || bc.z > 1.0f;

				// Discard if pixel not in triangle
				if (low || high)
					continue;

				// Depth is now within [near = 0.001, far = 10.0], comparison is LESS
				float depth = v0.z * bc.x + v1.z * bc.y + v2.z * bc.z;
				if (depth > GetDepth(*image, x, y))
					continue;
				SetDepth(image, x, y, depth);

				Vector3 p0 = positions[vertex + 0];
				Vector3 p1 = positions[vertex + 1];
				Vector3 p2 = positions[vertex + 2];
				Vector3 p = p0 * bc.x + p1 * bc.y + p2 * bc.z;
				
				Vector3 n0 = normals[vertex + 0];
				Vector3 n1 = normals[vertex + 1];
				Vector3 n2 = normals[vertex + 2];
				Vector3 n = n0 * bc.x + n1 * bc.y + n2 * bc.z;

				Color color = Float3ToColor(&bc.x);
				SetPixel(image, x, y, color);
			}
		}
	}

	delete[] rects;
	delete[] normals;
	delete[] positions;
	delete[] vertices;
}

struct UniformData
{
	Matrix mvp;
	Matrix world;
	Matrix normal;
	Vector3 cameraPos;
	Light light;

	// Supplying near & far for depth visualization
	float near;
	float far;
};

inline Matrix NormalMatrix(Matrix world)
{
	Matrix normal = world;
	normal.m12 = normal.m13 = normal.m14 = 0.0f;
	normal = Transpose(Invert(normal));
	return normal;
}
// Tri-linear interpolation across 3d points
inline Vector3 Terp(Vector3 A, Vector3 B, Vector3 C, Vector3 t)
{
	return A * t.x + B * t.y + C * t.z;
}

// Tri-linear interpolation across 2d points
inline Vector2 Terp(Vector2 A, Vector2 B, Vector2 C, Vector3 t)
{
	return A * t.x + B * t.y + C * t.z;
}
inline Vector3 DirectionalDiffuseLight(Vector3 normal, Vector3 direction, Color color, float diffuse)
{
	Vector3 Dir = { -direction.x, -direction.y, -direction.z };

	float dotNL = std::max(Dot(normal, Dir), 0.0f);

	Vector3 diffuseColor = {
		color.r * diffuse * dotNL,
		color.g * diffuse * dotNL,
		color.b * diffuse * dotNL
	};

	return diffuseColor;
}

inline Vector3 GetSpotLight(UniformData uniform, Vector3 n, Color textureColor, float depth, Vector3 p)
{
	Vector3 L = Normalize(uniform.light.position - p);
	Vector3 V = Normalize(uniform.cameraPos - p); 
	Vector3 R = Reflect(L * -1, n);
	Vector3 halfway = Normalize(V + L);


	float angle = Dot(L, {0,-90,0});

	if (angle > 45.0f)
	{
		float distance = Length(uniform.light.position - p);
		float attenuation = 1.0f / (distance * distance);
		float dotNL = std::max(Dot(n, L), 0.0f); //diffuse
		float dotVR = std::max(Dot(V, R), 0.0f); //spec
		float dotNH = std::max(Dot(halfway, n), 0.0f);//highlight

		Vector3 color = { textureColor.r, textureColor.g, textureColor.b };
		Vector3 d = V3_ONE * depth;
		color /= 255.0f;
		color *= d;
		color += uniform.light.ambient * dotNL;
		color += uniform.light.ambient * powf(dotNH, 1.0f);

		//attenuation
		color *= uniform.light.diffuse * attenuation;
		color *= uniform.light.radius; 

		return color;
	}

	return { 0.0f, 0.0f, 0.0f };
}
inline Vector3 GetPointLight(UniformData uniform, Vector3 n, Color textureColor, float depth, Vector3 p) {

	//phong I REALLY THINK THIS IS PHONG AND WORKS PERFECTLY RIGHT
// Light vector -- FROM fragment TO light
	Vector3 L = Normalize(uniform.light.position - p);
	Vector3 V = Normalize(uniform.cameraPos - p);
	Vector3 R = Reflect(L * -1, n);
	Vector3 halfway = Normalize(V + L);

	//dotnl
	float dotNL = std::max(Dot(n, L), 0.0f);
	float dotVR = std::max(Dot(V, R), 0.0f);
	float dotNH = std::max(Dot(halfway, n), 0.0f);

	float distance = Length(uniform.light.position - p);
	float attenuation = 1.0 / (distance * distance);

	Vector3 lightApply{ textureColor.r, textureColor.g, textureColor.b };
	Vector3 d = V3_ONE * depth;
	lightApply /= 255.0f;
	lightApply *= d;
	lightApply += uniform.light.ambient * dotNL;
	lightApply += uniform.light.ambient * powf(dotNH, 1.0f);
	lightApply *= uniform.light.diffuse * attenuation;
	lightApply *= uniform.light.radius;
	return lightApply;
}

inline void DrawMesh(Image* image, Mesh mesh, UniformData uniform, LightType lightType)
{
	// input
	Vector3* vertices = new Vector3[mesh.vertexCount];
	Vector3* ndcs = new Vector3[mesh.vertexCount];
	Vector3* positions = new Vector3[mesh.vertexCount];
	Vector3* normals = new Vector3[mesh.vertexCount];
	Vector2* tcoords = new Vector2[mesh.vertexCount];

	//vertex shader
	for (size_t i = 0; i < mesh.vertexCount; i++)
	{
		Vector4 clip;
		clip.x = mesh.positions[i].x;
		clip.y = mesh.positions[i].y;
		clip.z = mesh.positions[i].z;
		clip.w = 1.0f;

		clip = uniform.mvp * clip;
		clip /= clip.w;

		Vector3 ndc{ clip.x, clip.y, clip.z };
		Vector3 screen = ndc;
		screen.x = Remap(clip.x, -1.0f, 1.0f, 0.0f, image->width - 1.0f);
		screen.y = Remap(clip.y, -1.0f, 1.0f, 0.0f, image->height - 1.0f);

		vertices[i] = screen;
		ndcs[i] = ndc;
		positions[i] = uniform.world * mesh.positions[i];
		normals[i] = Normalize(uniform.normal * mesh.normals[i]);
		tcoords[i] = mesh.tcoords[i];
	}

	// Triangle AABBs
	Rect* rects = new Rect[mesh.faceCount];
	for (size_t face = 0; face < mesh.faceCount; face++)
	{
		// Ensure min & max get overwritten
		int xMin = image->width - 1;
		int yMin = image->height - 1;
		int xMax = 0;
		int yMax = 0;

		// Determine min & max of face, ensure its on-screen
		const size_t vertex = face * 3;
		for (size_t i = 0; i < 3; i++)
		{
			int x = vertices[vertex + i].x;
			int y = vertices[vertex + i].y;
			xMin = std::max(0, std::min(xMin, x));
			yMin = std::max(0, std::min(yMin, y));
			xMax = std::min(image->width - 1, std::max(xMax, x));
			yMax = std::min(image->height - 1, std::max(yMax, y));
		}
		// Face culling
		Vector3 v0 = ndcs[vertex + 0];
		Vector3 v1 = ndcs[vertex + 1];
		Vector3 v2 = ndcs[vertex + 2];
		Vector3 n = Normalize(Cross(v1 - v0, v2 - v1));
		bool front = Dot(n, V3_FORWARD) > 0.0f;
		if (front)
		{
			rects[face].xMin = xMin;
			rects[face].xMax = xMax;
			rects[face].yMin = yMin;
			rects[face].yMax = yMax;
		}
		else
		{
			rects[face].xMin = 0;
			rects[face].xMax = -1;
			rects[face].yMin = 0;
			rects[face].yMax = -1;
		}
	}

	for (size_t face = 0; face < mesh.faceCount; face++)
	{
		for (int x = rects[face].xMin; x <= rects[face].xMax; x++)
		{
			for (int y = rects[face].yMin; y <= rects[face].yMax; y++)
			{
				size_t vertex = face * 3;
				Vector3 v0 = vertices[vertex + 0];
				Vector3 v1 = vertices[vertex + 1];
				Vector3 v2 = vertices[vertex + 2];

				// Tri-linear interpolation, ensure 0.0 >= uvw <= 1.0
				Vector3 bc = Barycenter({ (float)x, (float)y, 0.0f }, v0, v1, v2);
				bool low = bc.x < 0.0f || bc.y < 0.0f || bc.z < 0.0f;
				bool high = bc.x > 1.0f || bc.y > 1.0f || bc.z > 1.0f;

				// Discard if pixel not in triangle
				if (low || high)
					continue;

				// Depth is now within [near = 0.001, far = 10.0], comparison is LESS
				float depth = v0.z * bc.x + v1.z * bc.y + v2.z * bc.z;
				if (depth > GetDepth(*image, x, y))
					continue;
				SetDepth(image, x, y, depth);

				// Fragment shader begin
				Vector3 p0 = positions[vertex + 0];
				Vector3 p1 = positions[vertex + 1];
				Vector3 p2 = positions[vertex + 2];
				Vector3 p = Terp(p0, p1, p2, bc);

				Vector3 n0 = normals[vertex + 0];
				Vector3 n1 = normals[vertex + 1];
				Vector3 n2 = normals[vertex + 2];
				Vector3 n = Terp(n0, n1, n2, bc);

				Vector2 uv0 = tcoords[vertex + 0];
				Vector2 uv1 = tcoords[vertex + 1];
				Vector2 uv2 = tcoords[vertex + 2];
				Vector2 uv = Terp(uv0, uv1, uv2, bc);

				float tw = gImageDiffuse.width;
				float th = gImageDiffuse.height;
				Color textureColor = GetPixel(gImageDiffuse, uv.x * tw, uv.y * th);
				Vector3 pixelColor;
				switch (lightType) {
				case (SPOT):
					pixelColor = GetSpotLight(uniform, n, textureColor, depth, p);
					break;
				case (DIRECTIONAL):
					pixelColor = DirectionalDiffuseLight(n, Vector3{0,0,-1}, textureColor, 1);
					break;
				case(POINT):
					pixelColor = GetPointLight(uniform, n, textureColor, depth, p);

					break;
				}

				Color color = Float3ToColor(&pixelColor.x);

				SetPixel(image, x, y, color);
			}
		}
	}


	delete[] rects;
	delete[] tcoords;
	delete[] normals;
	delete[] positions;
	delete[] ndcs;
	delete[] vertices;
}



