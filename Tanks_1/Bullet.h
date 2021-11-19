#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Player.h"

class Bullet
{
private:
    float damage;
    float posX;
    float posY;
    float widht;
    float height;
    float directionX;
    float directionY;
    float speed;
    bool isLive;
    float currentTimeMoveBullet;

    DirectionPlayer actualDirBullet;
    String nameFile;
    Image imageBullet;
    Texture textureBullet;
    Sprite spriteBullet;

public:

    bool GetIsLive();
    Sprite& GetSpriteBullet();
    Bullet(String NameFile, float PosX, float PosY, float Widht, float Height, DirectionPlayer ActualDirPlayer);
    void Update(float Time);
    void CollisionCheckMap();
    float GetPosX();
    float GetPosY();
    float GetDamage();
};

//----------------------------------------------------------------------\\

inline Bullet::Bullet(String NameFile, float PosX, float PosY, float Widht, float Height, DirectionPlayer ActualDirPlayer)
{
    damage = 40;
    currentTimeMoveBullet = 0;
    isLive = true;
    nameFile = NameFile;
    this->widht = Widht;
    this->height = Height;
    imageBullet.loadFromFile(NameFile);
    textureBullet.loadFromImage(imageBullet);
    spriteBullet.setTexture(textureBullet);
    spriteBullet.setOrigin(16, 16);
    posX = PosX; posY = PosY;
    spriteBullet.setTextureRect(IntRect(0, 0, Widht, Height));
    spriteBullet.setPosition(posX, posY);

    speed = 16;
    this->actualDirBullet = ActualDirPlayer;

    switch (actualDirBullet)
    {
    case  DirectionPlayer::RIGHT:
        directionX = speed;
        directionY = 0;
        spriteBullet.setRotation(0);
        break;
    case  DirectionPlayer::LEFT:
        directionX = (-1) * speed;
        directionY = 0;
        spriteBullet.setRotation(180);
        break;
    case  DirectionPlayer::DOWN:
        directionX = 0;
        directionY = speed;
        spriteBullet.setRotation(90);
        break;
    case  DirectionPlayer::UP:
        directionX = 0;
        directionY = (-1) * speed;
        spriteBullet.setRotation(270);
        break;
    }
    //posX += directionX;
    //posY += directionY;
    //spriteBullet.setPosition((int)posX, (int)posY);
}

inline void Bullet::Update(float Time)
{
    if (currentTimeMoveBullet > 0)
        currentTimeMoveBullet -= 0.15 * Time;
    else
    {
        posX += directionX;
        posY += directionY;

        spriteBullet.setPosition((int)posX, (int)posY);
        currentTimeMoveBullet = 10;
    }
}

inline void Bullet::CollisionCheckMap()
{

    for (int i = (((int)posY - 16) / 32); i <= (((int)posY) / 32); i++)
        for (int j = (((int)posX - 16) / 32); j <= (((int)posX) / 32); j++)
        {
            if (i < 0 || j < 0 || i >= HEIGHT_MAP || j >= WIDHT_MAP)
                continue;
            //cout << i << "   " << j << endl;
            if (arrMap[i][j].unitName != MapUnitName::FREE)
            {
                if (arrMap[i][j].unitName == MapUnitName::BOX)
                {
                    arrMap[i][j].health -= damage;
                }
                //cout << "Smert!\n";
                isLive = false;
            }
        }
}

inline float Bullet::GetPosX()
{
    return this->posX;
}

inline float Bullet::GetPosY()
{
    return this->posY;
}

inline float Bullet::GetDamage()
{
    return this->damage;
}

bool Bullet::GetIsLive()
{
    return this->isLive;
}

inline Sprite& Bullet::GetSpriteBullet()
{
    return this->spriteBullet;
}