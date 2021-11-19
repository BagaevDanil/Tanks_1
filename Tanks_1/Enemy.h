#pragma once
#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Player.h"

using namespace sf;
using namespace std;

class Enemy
{
private:
    float health;
    float posX;
    float posY;
    float widht;
    float height;
    float directionX;
    float directionY;
    float speed;
    float currentFrameAnimation;
    bool playerMove;
    float currentTimeMovePlayer;
    float currentTimeCreateBullet;
    DirectionPlayer actualDirPlayer;

    String nameFile;
    Image imagePlayer;
    Texture texturePlayer;
    Sprite spritePlayer;

public:


    Enemy(String NameFile, float PosX, float PosY, float Widht, float Height);
    void Update(float Time);
    void CollisionCheckMap();
    Sprite& GetSprite();
    float GetPosX();
    float GetPosY();
    bool GetPlayerMove();
    void SetPlayerMove(bool PlayerMove);
    DirectionPlayer GetaActualDirPlayer();
    void SetActualDirPlayer(DirectionPlayer ActualDirPlayer);
    float GetCurrentTimeCreateBullet();
    void SetCurrentTimeCreateBullet(float CurrentTimeCreateBullet);
    void SetHealth(float Health);
    float GetHealth();
};


//----------------------------------------------------------------------\\

Enemy::Enemy(String NameFile, float PosX, float PosY, float Widht, float Height)
{
    health = 100;
    actualDirPlayer = RIGHT;
    playerMove = true;
    currentTimeMovePlayer = 0;
    currentTimeCreateBullet = 0;
    playerMove = false;
    directionX = 0;
    directionY = 0;
    speed = 16;
    this->widht = Widht;
    this->height = Height;
    posX = PosX; posY = PosY;
    currentFrameAnimation = 0;

    nameFile = NameFile;
    imagePlayer.loadFromFile(NameFile);
    imagePlayer.createMaskFromColor(Color(255, 255, 255));
    texturePlayer.loadFromImage(imagePlayer);
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setOrigin(16, 16);
    spritePlayer.setTextureRect(IntRect(0, 0, Widht, Height));
    spritePlayer.setPosition(posX, posY);
    
}

inline void Enemy::Update(float Time)
{
    if (currentTimeMovePlayer > 0)
        currentTimeMovePlayer -= 0.05 * Time;

    if (currentTimeCreateBullet > 0)
        currentTimeCreateBullet -= 0.05 * Time;

    //cout << currentTimeCreateBullet << endl;;

    if (!playerMove || currentTimeMovePlayer > 0)
    {
        if (playerMove)
            playerMove = false;
        return;
    }
    playerMove = false;
    currentTimeMovePlayer = 10;

    currentFrameAnimation = int(++currentFrameAnimation) % 4;
    //cout << int(currentFrameAnimation) << endl;

    if ((int)posX % 32 == 16 && (int)posY % 32 == 16)
    {
        int i = ((int)posY - 16) / 32;
        int j = ((int)posX - 16) / 32;
        if (arrMap[i][j + 1].unitName == MapUnitName::FREE && actualDirPlayer != RIGHT && actualDirPlayer != LEFT && (rand() % 2))
            actualDirPlayer = RIGHT;
        else if (arrMap[i][j - 1].unitName == MapUnitName::FREE && actualDirPlayer != RIGHT && actualDirPlayer != LEFT && (rand() % 2))
            actualDirPlayer = LEFT;
        else if (arrMap[i + 1][j].unitName == MapUnitName::FREE && actualDirPlayer != UP && actualDirPlayer != DOWN && (rand() % 2))
            actualDirPlayer = DOWN;
        else if (arrMap[i - 1][j].unitName == MapUnitName::FREE && actualDirPlayer != UP && actualDirPlayer != DOWN && (rand() % 2))
            actualDirPlayer = UP;
    }

    switch (actualDirPlayer)
    {
    case DirectionPlayer::RIGHT:
        directionX = speed;
        directionY = 0;
        spritePlayer.setRotation(0);
        break;
    case DirectionPlayer::LEFT:
        directionX = (-1) * speed;
        directionY = 0;
        spritePlayer.setRotation(180);
        break;
    case DirectionPlayer::DOWN:
        directionX = 0;
        directionY = speed;
        spritePlayer.setRotation(90);
        break;
    case DirectionPlayer::UP:
        directionX = 0;
        directionY = (-1) * speed;
        spritePlayer.setRotation(270);
        break;
    }

    posX += directionX;
    posY += directionY;

    spritePlayer.setTextureRect(IntRect((32 * int(currentFrameAnimation)), 0, 32, 32));
    spritePlayer.setPosition((int)posX, (int)posY);
}

inline void Enemy::CollisionCheckMap()
{

    for (int i = (((int)posY - 16) / 32); i <= (((int)posY) / 32); i++)
        for (int j = (((int)posX - 16) / 32); j <= (((int)posX) / 32); j++)
        {
            if (i < 0 || j < 0 || i >= HEIGHT_MAP || j >= WIDHT_MAP)
                continue;
            //cout << i << "   " << j << endl;
            if (arrMap[i][j].unitName != MapUnitName::FREE)
            {
                //cout << i << "   " << j << endl;
                if (actualDirPlayer == DirectionPlayer::RIGHT)
                    posX -= 16;
                if (actualDirPlayer == DirectionPlayer::LEFT)
                    posX += 16;
                if (actualDirPlayer == DirectionPlayer::UP)
                    posY += 16;
                if (actualDirPlayer == DirectionPlayer::DOWN)
                    posY -= 16;
                actualDirPlayer = DirectionPlayer(rand() % 4);
                break;
            }
        }
    
    spritePlayer.setPosition((int)posX, (int)posY);
}

inline Sprite& Enemy::GetSprite()
{
    return (this->spritePlayer);
}

inline float Enemy::GetPosX()
{
    return this->posX;
}

inline float Enemy::GetPosY()
{
    return this->posY;
}

inline bool Enemy::GetPlayerMove()
{
    return this->playerMove;
}

inline void Enemy::SetPlayerMove(bool PlayerMove)
{
    this->playerMove = PlayerMove;
}

inline DirectionPlayer Enemy::GetaActualDirPlayer()
{
    return this->actualDirPlayer;
}

inline void Enemy::SetActualDirPlayer(DirectionPlayer ActualDirPlayer)
{
    this->actualDirPlayer = ActualDirPlayer;
}

inline float Enemy::GetCurrentTimeCreateBullet()
{
    return this->currentTimeCreateBullet;
}

inline void Enemy::SetCurrentTimeCreateBullet(float CurrentTimeCreateBullet)
{
    this->currentTimeCreateBullet = CurrentTimeCreateBullet;
}

inline void Enemy::SetHealth(float Health)
{
    this->health = Health;
}

inline float Enemy::GetHealth()
{
    return this->health;
}

