#pragma once
#include "Scene.h"
#include "Image.h"
#include "Texture.h"

class SecondScene : public Scene
{
public:
	void OnLoad() final;
	void OnUnload() final;

	void OnUpdate(float dt) final;
	void OnDrawImGui();
	void OnDraw() final;

private:
	Image mImage;
	Texture mTexture;
};
#pragma once
