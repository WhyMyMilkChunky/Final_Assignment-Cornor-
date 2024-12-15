#pragma once
#include "Scene.h"
#include "Image.h"
#include "Texture.h"
#include <imgui/imgui.h>

class FourthScene : public Scene
{
public:
	void OnLoad() final;
	void OnUnload() final;

	void OnUpdate(float dt) final;
	void OnDraw() final;

	void OnDrawImGui() final;
private:
	Image mImage;
	Texture mTexture;
};
#pragma once
