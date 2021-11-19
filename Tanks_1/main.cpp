#include <iostream>
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include <vector>
#include <sstream>

using namespace sf;
using namespace std;


void game(RenderWindow& window)
{
    //RenderWindow window(VideoMode(448, 448), "Tanks!", Style::Close);
    
    Player PlayerTank("Sprites/tanks/pla1.bmp", 48, 48, 32, 32);

    Texture stopGameTexture;
    stopGameTexture.loadFromFile("Sprites/menu/stopGame.png");
    Sprite stopGameSprite;
    stopGameSprite.setTexture(stopGameTexture);
    stopGameSprite.setTextureRect(IntRect(0, 0, 448, 448));
    stopGameSprite.setPosition(0, 0);

    Texture backgroundTexture;
    backgroundTexture.loadFromFile("Sprites/ground/ground02.bmp");
    Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setTextureRect(IntRect(0, 0, 448, 448));
    backgroundSprite.setPosition(0, 0);

    Texture healthTexture;
    healthTexture.loadFromFile("Sprites/health.png");
    Sprite healthSprite;
    healthSprite.setTexture(healthTexture);
    healthSprite.setTextureRect(IntRect(0, 0, 96, 32));
    healthSprite.setPosition(448-96, 0);

    Texture blocksTexture;
    blocksTexture.loadFromFile("Sprites/block/my1.png");
    Sprite blocksSprite;
    blocksSprite.setTexture(blocksTexture);

    vector <Bullet*> vectorBulletPlayer;
    vector <Bullet*> vectorBulletEnemy;
    vector <Enemy*> vectorEnemy;
    Clock mainClock;
    float currentTimeCreateBullet = 0;
    CreateMap();
    bool playGame = true;
    bool isWinGame = false;

    int numEnemy = 10;
    float currentSpawnEnemy = 500;

    Font font;
    font.loadFromFile("Sprites/PixeloidMono.ttf");
    Text endGameText("", font, 24);
    endGameText.setPosition(2, 2);
    endGameText.setFillColor(Color::Black);


    while (playGame)
    {
        //----------------------------TIME------------------------------------//
        float mainTime = mainClock.getElapsedTime().asMicroseconds();
        mainClock.restart();
        mainTime /= 800;
        stringstream ss;
        ss << numEnemy + vectorEnemy.size();
        endGameText.setString(ss.str());
        //----------------------------EXIT_BUTTON------------------------------------//
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        //----------------------------PLAYER1------------------------------------//
        if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            PlayerTank.SetPlayerMove(true);
            PlayerTank.SetActualDirPlayer(LEFT);
        }
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            PlayerTank.SetPlayerMove(true);
            PlayerTank.SetActualDirPlayer(RIGHT);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            PlayerTank.SetPlayerMove(true);
            PlayerTank.SetActualDirPlayer(UP);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            PlayerTank.SetPlayerMove(true);
            PlayerTank.SetActualDirPlayer(DOWN);
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && PlayerTank.GetCurrentTimeCreateBullet() <= 0)
        {
            vectorBulletPlayer.push_back(new Bullet("Sprites/bullet/bullet_1.png", PlayerTank.GetPosX(), PlayerTank.GetPosY(), 32, 32, PlayerTank.GetaActualDirPlayer()));
            PlayerTank.SetCurrentTimeCreateBullet(25);
        }
        if (currentSpawnEnemy <= 0 && numEnemy > 0)
        {
            vectorEnemy.push_back(new Enemy("Sprites/tanks/v5.bmp", 400, 400, 32, 32));
            currentSpawnEnemy= 200;
            numEnemy--;
        }

        PlayerTank.Update(mainTime);
        PlayerTank.CollisionCheckMap();
        window.clear();
        window.draw(backgroundSprite);
        
        if (currentSpawnEnemy > 0)
            currentSpawnEnemy -= 0.05f * mainTime;

        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDHT_MAP; j++)
            {
                arrMap[i][j].Update();
                arrMap[i][j].spriter.setPosition(j * 32, i * 32);
                window.draw(arrMap[i][j].spriter);

            }

        window.draw(PlayerTank.GetSprite());
        for (int i = 0; i < vectorBulletPlayer.size(); i++)
        {
            vectorBulletPlayer[i]->Update(mainTime);
            window.draw(vectorBulletPlayer[i]->GetSpriteBullet());
            vectorBulletPlayer[i]->CollisionCheckMap();
            if (!vectorBulletPlayer[i]->GetIsLive())
            {
                vectorBulletPlayer.erase(vectorBulletPlayer.begin() + i);
                continue;
            }

            bool check = true;
            for (int j = 0; j < vectorBulletEnemy.size(); j++)
            {
                if (check && vectorBulletEnemy[j]->GetPosX() == vectorBulletPlayer[i]->GetPosX() && (vectorBulletEnemy[j]->GetPosY() == vectorBulletPlayer[i]->GetPosY()))
                {
                    //vectorBulletEnemy[j]->SetHealth(vectorEnemy[j]->GetHealth() - vectorBulletPlayer[i]->GetDamage());
                    vectorBulletEnemy.erase(vectorBulletEnemy.begin() + j);
                    vectorBulletPlayer.erase(vectorBulletPlayer.begin() + i);
                    check = false;
                }
            }
            for (int j = 0; j < vectorEnemy.size(); j++)
            {
                if (check && vectorEnemy[j]->GetPosX() == vectorBulletPlayer[i]->GetPosX() && (vectorEnemy[j]->GetPosY() == vectorBulletPlayer[i]->GetPosY()))
                {
                    vectorEnemy[j]->SetHealth(vectorEnemy[j]->GetHealth() - vectorBulletPlayer[i]->GetDamage());
                    vectorBulletPlayer.erase(vectorBulletPlayer.begin() + i);
                    break;
                }
            }
        }

        for (int i = 0; i < vectorEnemy.size(); i++)
        {
            if (vectorEnemy[i]->GetHealth() <= 0)
            {
                vectorEnemy.erase(vectorEnemy.begin() + i);
                continue;
            }

            vectorEnemy[i]->SetPlayerMove(true);
            vectorEnemy[i]->Update(mainTime);
            window.draw(vectorEnemy[i]->GetSprite());
            vectorEnemy[i]->CollisionCheckMap();

            if (vectorEnemy[i]->GetCurrentTimeCreateBullet() <= 0)
            {
                if (rand() % 3)
                    vectorEnemy[i]->SetCurrentTimeCreateBullet(25);
                else
                {
                    vectorBulletEnemy.push_back(new Bullet("Sprites/bullet/bullet_1.png", vectorEnemy[i]->GetPosX(), vectorEnemy[i]->GetPosY(), 32, 32, vectorEnemy[i]->GetaActualDirPlayer()));
                    vectorEnemy[i]->SetCurrentTimeCreateBullet(25);
                }
                
            }
        }

        

        for (int i = 0; i < vectorBulletEnemy.size(); i++)
        {
            vectorBulletEnemy[i]->Update(mainTime);
            window.draw(vectorBulletEnemy[i]->GetSpriteBullet());
            vectorBulletEnemy[i]->CollisionCheckMap();
            if (!vectorBulletEnemy[i]->GetIsLive())
            {
                vectorBulletEnemy.erase(vectorBulletEnemy.begin() + i);
                continue;
            }

            if ((vectorBulletEnemy[i]->GetPosX() == PlayerTank.GetPosX()) && (vectorBulletEnemy[i]->GetPosY() == PlayerTank.GetPosY()))
            {
                PlayerTank.SetHealth(PlayerTank.GetHealth() - vectorBulletEnemy[i]->GetDamage());
                vectorBulletEnemy.erase(vectorBulletEnemy.begin() + i);
            }
                
        }

        if (PlayerTank.GetHealth() >= 100)
            healthSprite.setTextureRect(IntRect(0, 0, 96, 32));
        else if (PlayerTank.GetHealth() >= 60)
            healthSprite.setTextureRect(IntRect(0, 32, 96, 32));
        else if (PlayerTank.GetHealth() > 0)
            healthSprite.setTextureRect(IntRect(0, 64 , 96, 32));
        window.draw(healthSprite);

        //window.setView(mainCamera);
        

        if (PlayerTank.GetHealth() <= 0)
        {
            playGame = false;
        }

        if (numEnemy <= 0 && vectorEnemy.empty())
        {
            playGame = false;
            isWinGame = true;
        }

        if (!playGame)
        {
            if (!isWinGame)
                stopGameSprite.setTextureRect(IntRect(0, 0, 448, 448));
            else
                stopGameSprite.setTextureRect(IntRect(0, 448, 448, 448));
            stopGameSprite.setPosition(0, 0);
            window.draw(stopGameSprite);
            window.display();

            while (true)
            {
                if (Keyboard::isKeyPressed(Keyboard::Enter))
                {
                    break;
                }
            }
        }
        window.draw(endGameText);
        window.display();

    }

    //Font font;
    //font.loadFromFile("Sprites/PixeloidMono.ttf");
    //Text endGameText("", font, 40);
    //bool endGameActive = true;
    //if (!isWinGame)
    //    endGameText.setString("You Lose!\nPress ENTER...");
    //endGameText.setPosition(100, 216);

    //while (endGameActive)
    //{
    //    Event event;
    //    while (window.pollEvent(event))
    //    {
    //        if (event.type == Event::Closed)
    //            window.close();
    //    }

    //    if (Keyboard::isKeyPressed(Keyboard::Enter))
    //    {
    //        endGameActive = false;
    //    }

    //    window.clear();
    //    window.draw(endGameText);
    //    window.display();
    //}

    //return 0;
}

int main()
{
    //setlocale(0, "");

    RenderWindow window(VideoMode(448, 448), "Tanks!", Style::Close);

    Texture backGroundMenuTexture;
    backGroundMenuTexture.loadFromFile("Sprites/menu/bg.png");
    Sprite backGroundMenuSprite;
    backGroundMenuSprite.setTexture(backGroundMenuTexture);
    backGroundMenuSprite.setTextureRect(IntRect(0, 0, 448, 448));
    backGroundMenuSprite.setPosition(0, 0);

    Texture playMenuTexture;
    playMenuTexture.loadFromFile("Sprites/menu/playNew.png");
    Sprite playMenuSprite;
    playMenuSprite.setTexture(playMenuTexture);
    playMenuSprite.setTextureRect(IntRect(0, 0, 200, 64));
    playMenuSprite.setPosition(124, 224);

    Texture exitMenuTexture;
    exitMenuTexture.loadFromFile("Sprites/menu/exitNew.png");
    Sprite exitMenuSprite;
    exitMenuSprite.setTexture(exitMenuTexture);
    exitMenuSprite.setTextureRect(IntRect(0, 0, 200, 64));
    exitMenuSprite.setPosition(124, 326);

    int menuBut = 0;
    while (window.isOpen())
    {
        playMenuSprite.setTextureRect(IntRect(0, 0, 200, 64));
        exitMenuSprite.setTextureRect(IntRect(0, 0, 200, 64));
        //----------------------------EXIT_BUTTON------------------------------------//
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        if (IntRect(124, 224, 200, 64).contains(Mouse::getPosition(window)))
        {
            playMenuSprite.setTextureRect(IntRect(0, 64, 200, 64));
            menuBut = 1;
            //cout << "Play\n";
        }
        if (IntRect(124, 326, 200, 64).contains(Mouse::getPosition(window)))
        {
            exitMenuSprite.setTextureRect(IntRect(0, 64, 200, 64));
            menuBut = 2;
            //cout << "Exit\n";
        }


        if (Mouse::isButtonPressed(Mouse::Left))
        {
            if (menuBut == 2)
                window.close();
            else if (menuBut == 1)
            {
                window.clear();
                game(window);
            }
        }

        window.clear();
        window.draw(backGroundMenuSprite);
        window.draw(playMenuSprite);
        window.draw(exitMenuSprite);
        window.display();
    }
}