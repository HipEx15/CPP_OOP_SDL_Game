#include "GameObject.h"
#include "Texture.h"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(texturesheet);

	xpos = x;
	ypos = y;
}

GameObject::~GameObject()
{}

void GameObject::Update()
{
	xpos++;
	ypos++;

	srRect.h = 128;
	srRect.w = 128;

	srRect.x = 0;
	srRect.y = 0;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = srRect.w/2;
	destRect.h = srRect.h/2;
}

void GameObject::Render()
{
	SDL_RenderCopy(Game::renderer, objTexture, &srRect, &destRect);
}
