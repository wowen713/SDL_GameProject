#ifndef GameEngine_h
#define GameEngine_h

#include "game.h"
#include "player.h"
#include "Tiles.h"
#include "Background.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int LEVEL_WIDTH = 1920;
const int LEVEL_HEIGHT = 480;

const int Background_Size = 2;


const int TILE_WIDTH = 64;
const int TILE_HEIGHT = 64;
const int TOTAL_TILES = LEVEL_WIDTH/TILE_WIDTH;
const int TOTAL_TILES_SPRITES = 5;

const int TILE_GROUNDLEFT = 00;
const int TILE_GROUNDCENTER = 01;
const int TILE_GROUNDRIGHT = 02;
const int TILE_ROCK_LEFT = 03;
const int TILE_ROCK_RIGHT = 04;

class GameEngine{
public:
  void Init();
  
  bool Game_Is_Running();
  
  void HandleEvents();
  void UpdateMechanics();

  void SetCamera();
  void SetTiles();
  
  void Render();
  
  void Quit();

  
protected:
  Player* player;

  Background* BG[Background_Size];

  Tile* Tiles[TOTAL_TILES];
  
  
  SDL_Window* game_window = NULL;
  SDL_Renderer* game_renderer = NULL;
  SDL_Rect camera = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  
  bool is_running = true;
};

#endif
