#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

struct ClickedPosition{
    float x;
    float y;
};

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
        for(int i = 0 ; i < 5; ++i){
              vEnemy.push_back({float(15 + i * 35), 40.0f , true});
        }
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override{
        Clear(olc::BLACK);
        FillTriangle(fPlayerPositionX1, fPlayerPositionY1, fPlayerPositionX2, fPlayerPositionY2,fPlayerPositionX3, fPlayerPositionY3);
        if(GetKey(olc::Key::LEFT).bHeld){
            fPlayerPositionX1 = fPlayerPositionX1 - fPlayerVel;
            fPlayerPositionX2 = fPlayerPositionX2 - fPlayerVel;
            fPlayerPositionX3 = fPlayerPositionX3 - fPlayerVel;
        }
        if(GetKey(olc::Key::RIGHT).bHeld){
            fPlayerPositionX1 = fPlayerPositionX1 + fPlayerVel;
            fPlayerPositionX2 = fPlayerPositionX2 + fPlayerVel;
            fPlayerPositionX3 = fPlayerPositionX3 + fPlayerVel;
        }
        if(GetKey(olc::Key::UP).bHeld){
            fPlayerPositionY1 = fPlayerPositionY1 - fPlayerVel;
            fPlayerPositionY2 = fPlayerPositionY2 - fPlayerVel;
            fPlayerPositionY3 = fPlayerPositionY3 - fPlayerVel;
        }
        if(GetKey(olc::Key::DOWN).bHeld){
            fPlayerPositionY1 = fPlayerPositionY1 + fPlayerVel;
            fPlayerPositionY2 = fPlayerPositionY2 + fPlayerVel;
            fPlayerPositionY3 = fPlayerPositionY3 + fPlayerVel;
        }

        if(GetKey(olc::Key::SPACE).bPressed){
           float ftempX = fPlayerPositionX3;
           float ftempY = fPlayerPositionY3;
           vBullet.emplace_back(Bullet{ftempX, ftempY, false});
        }
        // detect collision for player's bullet against enemy.
        // is collision occurs kill both bullet and enemy.
        for (auto &elm : vBullet){
            for (auto &enemy : vEnemy){
               if (elm.x == enemy.x   && elm.y == enemy.y) elm.dead = true;
            }
            if(elm.y > -1 && !elm.dead){
                FillCircle(elm.x, elm.y, 1);
                elm.y = elm.y - fBulletVel;
            }
        }



        for(auto elm : vEnemy){
            FillCircle(elm.x, elm.y, 5);
        }


        return true;
    }
private:
    float fPlayerPositionX1 = 30.0f;
    float fPlayerPositionY1 = 30.0f;

    float fPlayerPositionX2 = 60.0f;
    float fPlayerPositionY2 = 30.0f;

    float fPlayerPositionX3 = 45.0f;
    float fPlayerPositionY3 = 20.0f;

    float fbulletPositionX = fPlayerPositionX3;
    float fbulletPositionY = fPlayerPositionY3;

    float fPlayerVel = 0.2f;
    float fBulletVel = 0.3f;

    std::vector<Bullet> vBullet;
    std::vector<Enemy>  vEnemy;
};

int main(){
    Transformation Demo;
    if (Demo.Construct(256, 240, 4, 4))
        Demo.Start();

    return 0;
}
