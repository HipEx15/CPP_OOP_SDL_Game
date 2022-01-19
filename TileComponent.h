#pragma once


#include "ECS.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include <SDL.h>

class TileComponent : public Component
{
public:

	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	TileComponent() = default;

	~TileComponent()
	{
		SDL_DestroyTexture(texture);
	}

	TileComponent(int srcX, int srcY, int xpos, int ypos, int tsize, int tscale, std::string id)
	{
		texture = Game::assets->GetTexture(id);

		srcRect.x = srcX;
		srcRect.y = srcY;
		srcRect.h = srcRect.w = tsize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.h = destRect.w = tsize * tscale;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}

	void setPos(int x, int y)
	{
		srcRect.x = x * 64;
		srcRect.y = y * 64;
	}
};
