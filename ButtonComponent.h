#pragma once

#include "Components.h"
#include <string>

class ButtonComponent : public Component
{
public:

    TransformComponent* transform;
    bool Hovered = false;
    int curx, cury;
    std::string tmp;
    bool Showed = false;
    bool alreadyPressed = false;

    float startX, startY;

    void init() override
    {
        transform = &entity->getComponent<TransformComponent>();
        startX = transform->position.x;
        startY = transform->position.y;
    }

    void update() override
    {
        transform->position.x = startX;
        transform->position.y = startY;
        used();
    }

    bool used()
    {
        SDL_GetMouseState(&curx, &cury);
        if ((curx > this->transform->position.x && curx < this->transform->position.x + this->transform->width)
            && (cury > this->transform->position.y && cury < this->transform->position.y + this->transform->height))
        {
            transform->position.x = startX - 10;
            transform->position.y = startY - 2.5;
            transform->scale = 0.9;
            Hovered = true;

            if (Game::event.button.state == SDL_PRESSED && this->alreadyPressed == 0 && Hovered == true)
            {
                this->alreadyPressed = true;
                std::cout << tmp << "\n";
                return true;
            }
        }
        else
        {
            Hovered = false;
            transform->scale = 1;
            transform->position.x = startX;
            transform->position.y = startY;
            return false;
        }

        if (Game::event.button.state == SDL_RELEASED)
            this->alreadyPressed = false;
    }

    ButtonComponent(std::string S)
    {
        tmp = S;
    }

    ButtonComponent() :transform(nullptr), curx(-1), cury(-1), startX(-1), startY(-1) {};

    void setMouseXY(int x, int y)
    {
        curx = x;
        cury = y;
    }

    /*
    void destroy()
    {
        transform->entity->destroy();
        transform = nullptr;
    }
    */
};