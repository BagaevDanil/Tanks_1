#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

const int HEIGHT_MAP = 14;
const int WIDHT_MAP = 14;

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
		nameFile = "Sprites/block/blocks2.png";
		image.loadFromFile(nameFile);
		image.createMaskFromColor(Color(0, 0, 0));
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
	"11111111111111",
	"10000000000221",
	"10101112211121",
	"12101002200121",
	"12101011110001",
	"10121021120021",
	"10221000000221",
	"12211122111221",
	"12210000001221",
	"10111222211101",
	"10100022000101",
	"10111222211101",
	"10000000000001",
	"11111111111111",
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