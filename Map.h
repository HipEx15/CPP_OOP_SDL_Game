#pragma once

#include <string>

class Map
{
public:
	Map(std::string tID, float ms, float ts);
	~Map();

	void LoadNewMap(std::string path, int sizeX, int sizeY);
	void LoadMap(std::string path, int sizeX, int sizeY);
	void AddTile(int srcX, int srcY, int xpos, int ypos);

private:
	std::string texID;
	float mapScale;
	int tileSize;
	float scaledSize;
};