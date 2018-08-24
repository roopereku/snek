#include<SDL2/SDL.h>
#include "Window.h"
#include "Renderer.h"
#include "Game.h"
#include "Config.h"
#include<time.h>
int main(int argc,char**argv){srand(time(0));if(!SDL_Init(SDL_INIT_VIDEO)){bool isRunning;Window win("",isRunning,true,4,2);Config config(argc,argv);Game game(win,config);while(isRunning){SDL_Event evnt;while(SDL_PollEvent(&evnt)){win.input(evnt,isRunning);game.input(evnt);}game.update();Render::setColor(0,0,0);Render::clearScreen();game.draw();Render::render();}SDL_Quit();}return 0;}