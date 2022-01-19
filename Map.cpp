#include <iostream>
#include <fstream>
#include "Map.h"
#include "Texture.h"
#include "Game.h"
#include "ECS.h"
#include "Components.h"

extern Manager manager;

Map::Map(std::string tID, float ms, float ts):texID(tID), mapScale(ms), tileSize(ts)
{
    scaledSize = ms * ts;
}

Map::~Map()
{
	
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
    char tilex, tiley, tile;

    std::fstream mapFile;

    mapFile.open(path);

    int srcX, srcY;


    for (int y = 0; y < sizeY; ++y)
    {
        for (int x = 0; x < sizeX; ++x)
        {
            mapFile.get(tilex);
            srcY = atoi(&tilex) * tileSize;
            mapFile.get(tiley);
            srcX = atoi(&tiley) * tileSize;
            AddTile(srcX, srcY, x * scaledSize, y * scaledSize);

            if (tilex == '0' && tiley == '4')
            {
                
                auto& pa(manager.addEntity());
                pa.addComponent<TransformComponent>(x * scaledSize, y * scaledSize, tileSize, tileSize, mapScale);
                pa.addComponent<ColliderComponent>("portalAlbastru");
                pa.addGroup(Game::groupPortal);
            }

            if (tilex == '0' && tiley == '7')
            {
               
                auto& pv(manager.addEntity());
                pv.addComponent<TransformComponent>(x * scaledSize, y * scaledSize, tileSize, tileSize, mapScale);
                pv.addComponent<ColliderComponent>("portalVerde");
                pv.addGroup(Game::groupPortal);
            }

            mapFile.ignore();
        }
    }

    mapFile.ignore();

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            if (tile == '1')
            {
                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("textures", x * scaledSize, y * scaledSize, scaledSize);
                tcol.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::LoadNewMap(std::string path, int sizeX, int sizeY)
{
    char tilex, tiley, tile;
    std::fstream mapFile;
    std::ios_base::iostate exceptionMask = mapFile.exceptions() | std::ios::failbit;
    mapFile.exceptions(exceptionMask);

    try
    {
        mapFile.open(path);
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

   

    int srcX, srcY;
    int counter = 0;
    auto& tiles(manager.getGroup(Game::groupMap));


    for (int y = 0; y < sizeY; ++y)
    {
        for (int x = 0; x < sizeX; ++x)
        {
            mapFile.get(tilex);
            srcY = atoi(&tilex) * tileSize;
            mapFile.get(tiley);
            srcX = atoi(&tiley) * tileSize;
            
            tiles[counter]->getComponent<TileComponent>().setPos(atoi(&tiley), atoi(&tilex));
            counter++;

            if (tilex == '0' && tiley == '4')
            {
                auto& pa(manager.addEntity());
                pa.addComponent<TransformComponent>(x * scaledSize, y * scaledSize, tileSize, tileSize, mapScale);
                pa.addComponent<ColliderComponent>("portalAlbastru");
                pa.addGroup(Game::groupPortal);
            }

            if (tilex == '0' && tiley == '7')
            {
                auto& pv(manager.addEntity());
                pv.addComponent<TransformComponent>(x * scaledSize, y * scaledSize, tileSize, tileSize, mapScale);
                pv.addComponent<ColliderComponent>("portalVerde");
                pv.addGroup(Game::groupPortal);
            }

            mapFile.ignore();
        }
    }

    mapFile.ignore();

    for (int y = 0; y < sizeY; y++)
    {
        for (int x = 0; x < sizeX; x++)
        {
            mapFile.get(tile);
            if (tile == '1')
            {
                auto& tcol(manager.addEntity());
                tcol.addComponent<ColliderComponent>("textures", x * scaledSize, y * scaledSize, scaledSize);
                tcol.addGroup(Game::groupColliders);
            }
            mapFile.ignore();
        }
    }

    mapFile.close();
}

void Map::AddTile(int srcX, int srcY, int xpos, int ypos)
{
    auto& tile(manager.addEntity());
    tile.addComponent<TileComponent>(srcX, srcY, xpos, ypos, tileSize, mapScale, texID);
    tile.addGroup(Game::groupMap);
}
