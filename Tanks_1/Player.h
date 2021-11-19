#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"

using namespace sf;
using namespace std;

enum DirectionPlayer
{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Player
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
    //bool canCreateBullet;
    DirectionPlayer actualDirPlayer;

    String nameFile;
    Image imagePlayer;
    Texture texturePlayer;
    Sprite spritePlayer;

public:
    

    Player(String NameFile, float PosX, float PosY, float Widht, float Height);
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

Player::Player(String NameFile, float PosX, float PosY, float Widht, float Height)
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

inline void Player::Update(float Time)
{
    //cout << health << endl;

    if (currentTimeMovePlayer > 0)
        currentTimeMovePlayer -= 0.05 * Time;

    if (currentTimeCreateBullet > 0)
        currentTimeCreateBullet -= 0.05 * Time;
    //else
        //canCreateBullet = true;

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

inline void Player::CollisionCheckMap()
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
                break;
            }
        }
    spritePlayer.setPosition((int)posX, (int)posY);
}

inline Sprite& Player::GetSprite()
{
    return (this->spritePlayer);
}

inline float Player::GetPosX()
{
    return this->posX;
}

inline float Player::GetPosY()
{
    return this->posY;
}

inline bool Player::GetPlayerMove()
{
    return this->playerMove;
}

inline void Player::SetPlayerMove(bool PlayerMove)
{
    this->playerMove = PlayerMove;
}

inline DirectionPlayer Player::GetaActualDirPlayer()
{
    return this->actualDirPlayer;
}

inline void Player::SetActualDirPlayer(DirectionPlayer ActualDirPlayer)
{
    this->actualDirPlayer = ActualDirPlayer;
}

inline float Player::GetCurrentTimeCreateBullet()
{
    return this->currentTimeCreateBullet;
}

inline void Player::SetCurrentTimeCreateBullet(float CurrentTimeCreateBullet)
{
    this->currentTimeCreateBullet = CurrentTimeCreateBullet;
}

inline void Player::SetHealth(float Health)
{
    this->health = Health;
}

inline float Player::GetHealth()
{
    return this->health;
}
