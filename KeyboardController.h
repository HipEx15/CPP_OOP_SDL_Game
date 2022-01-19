#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
    TransformComponent* transform;
    SpriteComponent* sprite;
    const Uint8* kbs = SDL_GetKeyboardState(NULL);
    int hdir = 0, vdir = 0;


    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        sprite = &entity->getComponent<SpriteComponent>();
    }


    void update() override
    {
        hdir = (bool)kbs[SDL_SCANCODE_D] - (bool)kbs[SDL_SCANCODE_A];
        vdir = (bool)kbs[SDL_SCANCODE_S] - (bool)kbs[SDL_SCANCODE_W];

        if (hdir || vdir) sprite->Play("Walk");

        if (hdir == -1)
        {
            sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            sprite->spriteFlip = SDL_FLIP_NONE;
        }

        if (hdir == 0 && vdir == 0)
        {
            transform->velocity.x = 0;
            transform->velocity.y = 0;
            sprite->Play("Idle");
        }
        else if (hdir != 0 && vdir != 0)
        {
            transform->velocity.x = hdir * 0.71;
            transform->velocity.y = vdir * 0.71;
        }
        else
        {
            transform->velocity.x = hdir;
            transform->velocity.y = vdir;
        }
    }

};