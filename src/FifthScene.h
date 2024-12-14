#pragma once
#include "Scene.h"
#include "Image.h"
#include "Texture.h"

class FifthScene : public Scene
{
public:
	void OnLoad() final;
	void OnUnload() final;

	void OnUpdate(float dt) final;
	void OnDraw() final;

private:
	Image mImage;
	Texture mTexture;
};
#pragma once
