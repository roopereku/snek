#include "SnakeHandler.h"
SnakeHandler::SnakeHandler(WorldSpace&ws,Config&config){constexpr int keys[][2]{{SDLK_LEFT,SDLK_RIGHT},{SDLK_n,SDLK_m},{SDLK_z,SDLK_x},{SDLK_j,SDLK_k}};const Vector2 origin[]{Vector2(-0.2f,-0.2f),Vector2(-0.2f,+0.2f),Vector2(+0.2f,+0.2f),Vector2(+0.2f,-0.2f)};const int maxPlayers=config.fromSingle("-maxplayers");int numPlayers=config.fromSingle("-players");if(numPlayers>maxPlayers){numPlayers=maxPlayers;}for(int i=0;i<numPlayers;i++){int keyL=i<4 ? keys[i][0] : 0;int keyR=i<4 ? keys[i][1] : 0;Vector2 position=i<4 ? origin[i] : Vector2(0.0f,0.0f);add(Snake(i,position,ws,keyL,keyR,config));}initialSnakeAmount=snakes.size();}void SnakeHandler::add(Snake snake){snakes.push_back(snake);}void SnakeHandler::input(SDL_Event evnt){for(size_t i=0;i<snakes.size();i++)snakes[i].input(evnt);}void SnakeHandler::draw(){for(size_t i=0;i<snakes.size();i++)snakes[i].draw();}bool SnakeHandler::update(Borders&borders,PointHandler&point){std::vector<bool>snakeStates(snakes.size());for(size_t i=0;i<snakes.size();i++)snakeStates[i]=snakes[i].update(borders,point,snakes);if(initialSnakeAmount==1){return snakeStates[0];}std::vector<size_t>successes;int successfulReturns=0;for(size_t i=0;i<snakeStates.size();i++){if(snakeStates[i]==true){successfulReturns++;successes.push_back(i);}}if(successfulReturns<=1){return false;}return true;}int SnakeHandler::initialSnakes(){return initialSnakeAmount;}