#pragma once

#include "Components.h"
#include "ECS.h"
#include "Game.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TimerComponent : public Component
{
private:
    TTF_Font* font;
    SDL_Rect position;
    std::string labelText;
    std::string labelFont;
    SDL_Color textColour;
    SDL_Texture* labelTexture;

public:

    TimerComponent(int xpos, int ypos, std::string text, int size, SDL_Color& colour) :
        labelText(text), textColour(colour)
    {
        font = TTF_OpenFont("Images/FFFFORWA.TTF", size);
        position.x = xpos;
        position.y = ypos;

        SetLabelText(labelText);

    }

    ~TimerComponent()
    {
        
        if(font != nullptr)
        {
            TTF_CloseFont(font);
            font = nullptr;
        }
        
    }

    void init() override
    {
    }

    void SetLabelText(std::string text)
    {

        SDL_Surface* Surface = TTF_RenderText_Blended(font, text.c_str(), textColour);
        SDL_DestroyTexture(labelTexture);
        labelTexture = SDL_CreateTextureFromSurface(Game::renderer, Surface);
        SDL_FreeSurface(Surface);

        SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
    }

    void setPos(int x, int y)
    {
        position.x = x;
        position.y = y;
    }

    void draw() override
    {
        SDL_RenderCopy(Game::renderer, labelTexture, nullptr, &position);
    }
};