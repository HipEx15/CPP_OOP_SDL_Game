#pragma once

#include<string>
#include<SDL.h>
#include "Components.h"
#include "TransformComponent.h"
#include "Texture.h"

class ColliderComponent : public Component
{
public:

	SDL_Rect collider;
	std::string tag;

	SDL_Texture* tex;
	SDL_Rect srcR, destR;

	TransformComponent* transform;

	ColliderComponent(std::string T)
	{
		tag = T;
	}

	ColliderComponent(std::string T, int xpos, int ypos, int size)
	{
		tag = T;
		collider.x = xpos;
		collider.y = ypos;
		collider.h = collider.w = size;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}
		transform = &entity->getComponent<TransformComponent>();

		tex = TextureManager::LoadTexture("Images/Wall.png");
		srcR = { 0,0,64,64 };
		destR = {collider.x, collider.y, collider.w, collider.h};
	}

	void update() override
	{
		if (tag != "textures")
		{
			collider.x = static_cast<int>(transform->position.x + 16);
			collider.y = static_cast<int>(transform->position.y + 16);
			collider.w = (transform->width - 32) / transform->scale ;
			collider.h = (transform->height - 32)/ transform->scale ;
		}
	}

	void draw() override
	{
		TextureManager::Draw(tex, srcR, destR, SDL_FLIP_NONE);
	}


};