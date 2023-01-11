#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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


class Transformation : public olc::PixelGameEngine{
public:
    Transformation(){
        sAppName = "Two Dimensional Transformation";
    }

    bool OnUserCreate() override{
        for(int i = 0 ; i < 1; ++i){
              vEnemy.push_back({float(ScreenWidth() / 2 + i * 75 - 150), 40.0f , true});
        }

        fPlayerPositionX = ScreenWidth() / 2;
        fPlayerPositionY = ScreenHeight() / 2;

        sprEnemy = std::make_unique<olc::Sprite>("/home/abhilekh/Downloads/enemy.png");
        sprPlayer = std::make_unique<olc::Sprite>("/home/abhilekh/Downloads/player.png");

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override{
        if(!GetKey(olc::Key::ENTER).bPressed && count == 0){
            DrawString(ScreenWidth() /2 - 100, ScreenHeight() / 2 - 40, "Space Warrior",olc::WHITE,3);
            DrawString(ScreenWidth() /2 - 100, ScreenHeight() / 2 + 50, "By: Abhilekh Gautam",olc::WHITE,2);
            DrawString(ScreenWidth() /2 - 100, ScreenHeight() / 2 + 80, "Press Enter to Continue",olc::WHITE,1);

        }


        else{
            count = 1;
            Clear(olc::BLACK);
            //FillTriangle(fPlayerPositionX1, fPlayerPositionY1, fPlayerPositionX2, fPlayerPositionY2,fPlayerPositionX3, fPlayerPositionY3);

            DrawSprite(olc::vi2d(fPlayerPositionX, fPlayerPositionY), sprPlayer.get());

            if(GetKey(olc::Key::LEFT).bHeld){
                fPlayerPositionX = fPlayerPositionX - fPlayerVel;
            }
            if(GetKey(olc::Key::RIGHT).bHeld){
                fPlayerPositionX = fPlayerPositionX + fPlayerVel;
            }
            if(GetKey(olc::Key::UP).bHeld){
                fPlayerPositionY = fPlayerPositionY - fPlayerVel;
            }
            if(GetKey(olc::Key::DOWN).bHeld){
                fPlayerPositionY = fPlayerPositionY + fPlayerVel;
            }

            if(GetKey(olc::Key::SPACE).bPressed){
                float ftempX = fPlayerPositionX;
                float ftempY = fPlayerPositionY;
                vBullet.emplace_back(Bullet{ftempX + sprPlayer->width / 2, ftempY, false});
                vEBullet.emplace_back(Bullet{vEnemy[0].x, vEnemy[0].y + ScreenHeight() / 2, false});
            }
            // detect collision for player's bullet against enemy.
            // if collision occurs kill both bullet and enemy.
            for (auto &elm : vBullet){
                for (auto &enemy : vEnemy){
                    if (enemy.alive && elm.x - 1 >= enemy.x - sprEnemy->width / 2 && elm.x + 1 <= enemy.x + sprEnemy->width / 2 && elm.y - 1 >= enemy.y + sprEnemy ->height / 2 ){
                        elm.dead = true;
                        enemy.alive = false;
                    }
                }
                if(elm.y > -1 && !elm.dead){
                    FillCircle(elm.x, elm.y, 2, olc::RED);
                    elm.y = elm.y - fBulletVel;
                }
            }
            // todo: collision detection : enemy bullet vs player

            // fire bullet: enemy
            for(auto elm : vEBullet){
                if(!elm.dead){
                    FillCircle(elm.x, elm.y, 2, olc::BLUE);
                    elm.y = elm.y - fBulletVel;
                }
            }

            // load enemy here
            for(auto elm : vEnemy){
             //FillCircle(elm.x, elm.y, 8);
             if(elm.alive)
              DrawSprite(olc::vi2d(elm.x, elm.y), sprEnemy.get());
            }
        }



        return true;
    }
private:

    float fPlayerPositionX = 185.0f;
    float fPlayerPositionY = 220.0f;

    float fbulletPositionX = fPlayerPositionX;
    float fbulletPositionY = fPlayerPositionY;

    float fPlayerVel = 0.2f;
    float fBulletVel = 0.3f;

    std::vector<Bullet> vBullet;
    std::vector<Enemy>  vEnemy;
    std::vector<Bullet> vEBullet;
    int count = 0;

    std::unique_ptr<olc::Sprite> sprEnemy;
    std::unique_ptr<olc::Sprite> sprPlayer;

};

int main(){
    Transformation Demo;
    if (Demo.Construct(450, 340, 4, 4))
        Demo.Start();

    return 0;
}
