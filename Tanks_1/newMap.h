#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

const int HEIGHT_MAP = 12;
const int WIDHT_MAP = 12;

enum MapUnitName
{
	WALL = 1,
	BOX = 2,
	FREE = 0
};

class MapUnit
{
public:
	MapUnitName unitName;
	float health;
	String nameFile;
	Image image;
	Texture texture;
	Sprite spriter;

	MapUnit()
	{
		nameFile = "Sprites/block/blocks.png";
		image.loadFromFile(nameFile);
		image.createMaskFromColor(Color(255, 255, 255));
		texture.loadFromImage(image);
		spriter.setTexture(texture);
		health = 100;
	}

	void Update()
	{
		switch (unitName)
		{
		case WALL:
			spriter.setTextureRect(IntRect(0, 0, 32, 32));
			break;
		case BOX:
			if (health >= 100)
				spriter.setTextureRect(IntRect(32, 0, 32, 32));
			else if (health >= 60)
				spriter.setTextureRect(IntRect(64, 0, 32, 32));
			else if (health > 0)
				spriter.setTextureRect(IntRect(96, 0, 32, 32));
			else
			{
				unitName = MapUnitName::FREE;
				spriter.setTextureRect(IntRect(128, 0, 32, 32));
			}
			break;
		case FREE:
			spriter.setTextureRect(IntRect(128, 0, 32, 32));
			break;
		}
	}

};

String arr[HEIGHT_MAP] =
{
	"111111111111",
	"100000000201",
	"100000000201",
	"101112211101",
	"102000000001",
	"101122211101",
	"100000000001",
	"101122221101",
	"101002000101",
	"101112000101",
	"100000000001",
	"111111111111",
};

MapUnit arrMap[HEIGHT_MAP][WIDHT_MAP];

void CreateMap()
{
	for (int i = 0; i < HEIGHT_MAP; i++)
	{
		for (int j = 0; j < WIDHT_MAP; j++)
		{
			if (arr[i][j] - '0' == MapUnitName::WALL)
			{
				arrMap[i][j].unitName = WALL;
			}
			if (arr[i][j] - '0' == MapUnitName::BOX)
			{
				arrMap[i][j].unitName = BOX;
			}
			if (arr[i][j] - '0' == MapUnitName::FREE)
			{
				arrMap[i][j].unitName = FREE;
			}
		}
	}
}