/*

  Space Warrior is a 2D game written by Abhilekh Gautam. It uses C++
  and olc pixel game engine for drawing and olc sound wave engine for music stuff

  For more about olc Pixel Game engine visit: https://github.com/OneLoneCoder/olcPixelGameEngine
  For more about olc sound wave engine visit: https://github.com/OneLoneCoder/olcSoundWaveEngine


 */
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_SOUNDWAVE
#include "olcSoundWaveEngine.h"

/*
 I Really hate this approach of coding, everything in a single file will implement soon it in
 entity component model....

  Did this because I wanted to get things done quickly.

  Collision Detection can be greatly improved. I simply used crude collision detection
  technique that worked pretty well..

  Yuck.. this approach

 */

struct Bullet{
    float x;
    float y;
    bool dead;
};

struct Enemy{
    float x;
    float y;
    bool alive;
};


class Game : public olc::PixelGameEngine {
public:
    Game() {
        sAppName = "Space Warrior";
    }

    // set enemy to their default position..
    void produceEnemy() {
        for (int i = 0; i < 70; ++i) {
            if (i < 18)
                vEnemy.emplace_back(Enemy{float(ScreenWidth()) / 2 + (float) i * 10 - 100, 40.0f, true});
            else if (i < 36)
                vEnemy.emplace_back(Enemy{float(ScreenWidth()) / 2 + 10.0f * (float) i - 280, 55.0f, true});
            else if (i < 54)
                vEnemy.emplace_back(Enemy{float(ScreenWidth()) / 2 + 10.0f * (float) i - 460, 75.0f, true});
            else
                vEnemy.emplace_back(Enemy{float(ScreenWidth()) / 2 + 10.0f * (float) i - 640, 95.0f, true});
        }
    }

    void reset(){
        vEnemy.clear(); // clear all enemy
        vBullet.clear(); // clear all bullet stuff
        produceEnemy();
        life_count = 3;
        score = 0;
        fPlayerPositionX = 185.0f;
        fPlayerPositionY = 250.0f;
    }

    bool OnUserCreate() override {

        bgmEngine.InitialiseAudio();
        // background music
        bgm = std::make_unique<olc::sound::Wave>("../music/bgm.wav");
        gunShot = std::make_unique<olc::sound::Wave>("../music/gunshot.wav");
        blast = std::make_unique<olc::sound::Wave>("../music/enemyDeath.wav");

        produceEnemy();

        // sprites...
        sprEnemy = std::make_unique<olc::Sprite>("../sprites/enemy9.png");
        sprPlayer = std::make_unique<olc::Sprite>("../sprites/player.png");
        sprExplosion = std::make_unique<olc::Sprite>("../sprites/blast.png");
        sprLife = std::make_unique<olc::Sprite>("../sprites/life-full.png");

        // play music
        bgmEngine.PlayWaveform(bgm.get(), true);
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override {
        // initial user prompt, wait for enter key to be pressed...
        if (!GetKey(olc::Key::ENTER).bPressed && count == 0) {
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 - 40, "Space Warrior", olc::WHITE, 3);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 + 50, "By: Abhilekh Gautam", olc::WHITE, 2);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 + 80, "Press Enter to Continue", olc::WHITE, 1);
        } else if (life_count > 0 && life_count <= 3 && score < 350) {
            count = 1;
            Clear(olc::BLACK);

            // draw player
            DrawSprite(olc::vi2d((int) fPlayerPositionX, (int) fPlayerPositionY), sprPlayer.get());

            // wait for certain keys to be pressed

            if (GetKey(olc::Key::LEFT).bHeld) {
                if (fPlayerPositionX > 0.0f)
                    fPlayerPositionX = fPlayerPositionX - fPlayerVel * fElapsedTime;
            }
            if (GetKey(olc::Key::RIGHT).bHeld) {
                if (fPlayerPositionX + (float)sprPlayer->width < (float) ScreenWidth() - 10)
                    fPlayerPositionX = fPlayerPositionX + fPlayerVel * fElapsedTime;
            }
            if (GetKey(olc::Key::UP).bHeld) {
                fPlayerPositionY = fPlayerPositionY - fPlayerVel * fElapsedTime;
            }
            if (GetKey(olc::Key::DOWN).bHeld) {
                if (fPlayerPositionY + (float)sprPlayer->height < (float) ScreenHeight())
                    fPlayerPositionY = fPlayerPositionY + fPlayerVel * fElapsedTime;
            }

            // shoot
            if (GetKey(olc::Key::SPACE).bPressed) {
                bgmEngine.PlayWaveform(gunShot.get(), false);
                float ftempX = fPlayerPositionX;
                float ftempY = fPlayerPositionY;
                vBullet.emplace_back(Bullet{ftempX + float(sprPlayer->width) / 2, ftempY, false});
            }

            // detect collision for player's bullet against enemy.
            for (auto &elm: vBullet) {
                for (auto &enemy: vEnemy) {
                    if (!elm.dead && enemy.alive && elm.y > enemy.y && elm.x + 1 >= enemy.x &&
                        elm.x - 1 <= enemy.x + float(sprEnemy->width) &&
                        elm.y - 1 <= enemy.y + float(sprEnemy->height)) {
                        // kill both bullet and enemy.
                        DrawSprite(olc::vi2d(int(enemy.x), int(enemy.y)), sprExplosion.get());
                        bgmEngine.PlayWaveform(blast.get(), false);
                        elm.dead = true;
                        enemy.alive = false;
                        score = score + 5;
                    }
                }
                // only take care of bullets which are visible on the screen
                if (elm.y > -1 && !elm.dead) {
                    FillCircle(int(elm.x), int(elm.y), 1, olc::RED);
                    elm.y = elm.y - fBulletVel * fElapsedTime;
                }
            }


            // move random enemy towards the player only if the enemy is alive
            if (vEnemy[index].alive) {
                // distance calculation between enemy and player..
                float tempX = (vEnemy[index].x + float(sprEnemy->height) + float(sprEnemy->width) / 2 -
                               (fPlayerPositionX + float(sprPlayer->width) / 2 - 5
                               ));
                float tempY = (vEnemy[index].y + float(sprEnemy->height) - fPlayerPositionY);

                // simple pythagoras theorem
                float tempHypo = powf(tempX, 2) + powf(tempY, 2);
                float Hypo = sqrtf(tempHypo);
                // angle stuffs
                float sinTheta = (tempY / Hypo);
                float cosTheta = (tempX / Hypo);

                // simply moves enemy towards player..
                vEnemy[index].x = vEnemy[index].x - fEnemyVel * cosTheta * fElapsedTime;
                vEnemy[index].y = vEnemy[index].y - fEnemyVel * sinTheta * fElapsedTime;

                // enemy vs player collision detection
                // increase score
                // reduce life
                if (vEnemy[index].x + float(sprEnemy->width) >= fPlayerPositionX &&
                    vEnemy[index].x <= fPlayerPositionX + float(sprPlayer->width) &&
                    vEnemy[index].y + float(sprEnemy->height) >= fPlayerPositionY) {
                    bgmEngine.PlayWaveform(blast.get(), false);
                    vEnemy[index].alive = false;
                    score = score + 5;
                    life_count = life_count - 1;
                }
            } else {
                // sorry rand() is too short and attractive.. ;)
                index = rand() % 70;
            }

            // Draw enemy here..
            for (auto elm: vEnemy) {
                if (elm.alive)
                    DrawSprite(olc::vi2d(int(elm.x), int(elm.y)), sprEnemy.get());
            }
            // Score board ..
            DrawString(0, 5, "SCORE:" + std::to_string(score));

            // load life sprite..
            for (int i = 0; i < life_count; ++i) {
                DrawSprite(olc::vi2d(270 + sprLife->width + 25 * i, 5), sprLife.get());
            }

        } else if (life_count <= 0 && score < 350) { // player lost
            Clear(olc::BLACK);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 - 30, "You Lose!!!", olc::WHITE, 3);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 + 60, "Press Enter to Restart", olc::WHITE, 1);
            //reset everything
            if (GetKey(olc::Key::ENTER).bPressed) {
                reset();
            }
        } else { // player won
            Clear(olc::BLACK);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 - 30, "You Won!!!", olc::WHITE, 3);
            DrawString(ScreenWidth() / 2 - 100, ScreenHeight() / 2 + 60, "Press Enter to Restart", olc::WHITE, 1);
            // reset everything
            if (GetKey(olc::Key::ENTER).bPressed) {
                reset();
            }
        }

        return true;
    }

private:

    // players starting position
    float fPlayerPositionX = 185.0f;
    float fPlayerPositionY = 250.0f;

    // game universe constants
    float fPlayerVel = 90.0f;
    float fBulletVel = 180.0f;
    float fEnemyVel = 180.0f;

    std::vector<Bullet> vBullet;
    std::vector<Enemy> vEnemy;

    int count = 0;
    int life_count = 3;

    int score = 0;
    // choose random enemy first (for movement)
    int index = rand() % 70;

    // sprite stuffs...
    std::unique_ptr<olc::Sprite> sprEnemy;
    std::unique_ptr<olc::Sprite> sprPlayer;
    std::unique_ptr<olc::Sprite> sprExplosion;
    std::unique_ptr<olc::Sprite> sprLife;

    // music stuffs..
    olc::sound::WaveEngine bgmEngine;
    std::unique_ptr<olc::sound::Wave> bgm;
    std::unique_ptr<olc::sound::Wave> gunShot;
    std::unique_ptr<olc::sound::Wave> blast;
};

int main(){
    Game game;
    if (game.Construct(450, 340, 4, 4))
        game.Start();

    return 0; // happy compiler
}
