#pragma once
#include "Components.h"
#include "Vector2D.h"

class TransformComponent : public Component
{
public:

	Vector2D position;
	Vector2D velocity;

	int height = 128;
	int width = 128;
	float scale = 2;

	int speed = 3;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float sc)
	{
		position.x = 0.0f;
		position.y = 0.0f;
		scale = sc;
	}

	TransformComponent(float x, float y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(float x, float y, int h, int w, float sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
	}

	void init() override
	{
		velocity.Zero();
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}

	void setPos(float x, float y)
	{
		position.x = x;
		position.y = y;
	}
};