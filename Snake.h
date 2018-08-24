#ifndef SNAKE_H
#define SNAKE_H
#include "WorldSpace.h"
#include "PointHandler.h"
#include "Borders.h"
#include "Vector2.h"
#include "Config.h"
#include<SDL2/SDL.h>
#include<vector>
#include<map>
class Snake{public:Snake(int id,Vector2 origin,WorldSpace&ws,int keyLeft,int keyRight,Config&config);void addLength(int amount);void input(SDL_Event evnt);bool update(Borders&borders,PointHandler&point,std::vector<Snake>&other);Vector2 getHeadPosition();void kill();bool isImmortal();void draw();private:std::vector<Vector2>parts;int snakeID;float rotation;float speed;float acceleration;unsigned long long score;bool alive;float immortalityTimer;float immortalityTimerMax;unsigned char r;unsigned char g;unsigned char b;struct Key{bool pressed=false;float effect=0.0f;};std::map<int,Key>keys;WorldSpace&ws;};
#endif