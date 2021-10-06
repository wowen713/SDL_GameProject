#include "GameEngine.h"
#include "player.h"
#include "Tiles.h"


void GameEngine::Init(){
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { 
    std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl; 
  }
  //Enable gpu_enhanced textures
  IMG_Init(IMG_INIT_PNG);
  
  game_window = SDL_CreateWindow("my_game", 
			       SDL_WINDOWPOS_CENTERED, 
			       SDL_WINDOWPOS_CENTERED, 
			       SCREEN_WIDTH,
			       SCREEN_HEIGHT, 0);
  game_renderer = SDL_CreateRenderer(game_window,-1,0);
  player = new Player(game_renderer); //create the player
  player->Obj_Init("../Assignment_3/Images/Player_sprite_sheet2.png", 1, 60, 50, (SCREEN_HEIGHT - 264 ), 32, 32, 80, 220);
  player->set_state("IDLE");
  BG[0] = new Background(game_renderer); //create the ground
  BG[0]->BG_Init("../Assignment_3/Images/looping_trees.png", 384, 216, 0, 384/2 , 216, 1);
  BG[1] = new Background(game_renderer);
  BG[1]->BG_Init("../Assignment_3/Images/looping_mountains.png", 1970, 595, 0, 1970/3, 595, 2);
  SetTiles();
  //std::cout << "player created with renderer" << std::endl;
}

bool GameEngine::Game_Is_Running(){
  return is_running; //for checking if game is running
}

void GameEngine::HandleEvents(){
  SDL_Event input;
  string state_choice;
  SDL_PollEvent(&input);
  if(input.type == SDL_QUIT) is_running = false;  
  //if(!(player->get_is_jumping())){   //As long as the player isnt in the air take inputs
  if(input.type == SDL_KEYDOWN){
    switch(input.key.keysym.sym){
      /*case SDLK_LEFT :  //move left
      state_choice = "RUNLEFT";
      std::cout << "left" << std::endl;
      player->set_state(state_choice);
      break;*/
    case SDLK_RIGHT :  //move right
      state_choice = "RUNRIGHT";
      std::cout << "right" << std::endl;
      player->set_state(state_choice);
      break;
    case SDLK_SPACE :    //jump
      if(!(player->get_is_jumping())){ 
      //std:: cout << player->get_state() << std::endl;
      player->set_prev_state(player->get_state());
      state_choice = "JUMP";
      std::cout << "up" << std::endl;
      //std::cout << player->get_prev_state() << std::endl;
      player->set_state(state_choice);
      break;
      }
    }
  }
  else if(input.type == SDL_KEYUP){ //if a key is lifted then switch to idle from that key
    std::cout << "keyup" << std::endl;
      switch(input.key.keysym.sym){
      case SDLK_LEFT :  //move left
	if(player->get_state() == "RUNLEFT"){
	  state_choice = "IDLE";
	  std::cout << "idle from left up" << std::endl;
	  player->set_state(state_choice);
	  break;
	}
      case SDLK_RIGHT :  //move right
	if(player->get_state() == "RUNRIGHT"){
	  state_choice = "IDLE";
	  std::cout << "idle from right up" << std::endl;
	  player->set_state(state_choice);
	  break;
	}
      }
    }
      /* default:     //for when no buttons are being pressed for when I use Del_vel
      state_choice = "IDLE";
      std::cout << "idle" << std::endl;
      player->set_state(state_choice);

      //this following section is for when I can implement all movement using DEL_vel rather than having to do two different methods
      int temp_y = player->get_y_pos() + player->get_height();
      int temp_x = player->get_x_vel();
      if(temp_x < 0){
      player->set_x_vel(temp_x + 1);
      }
      if(temp_x > 0){
      player->set_x_vel(temp_x - 1);
      }
      //different because of position metaphor
      //faking gravity if jump input is ended before apex of jump
      if(temp_y > SCREEN_HEIGHT){
      //player->set_y_pos(temp_y + 5);
      }
      if(temp_y < SCREEN_HEIGHT){
      player->set_y_pos(SCREEN_HEIGHT - player->get_height());
      }*/
}

void GameEngine::UpdateMechanics(){
  //std::cout << "obj update call" << std::endl;
  player->Obj_Update(); //call update on object
  for(int i = Background_Size - 1; i >= 0; i--){ //update the background elements
    BG[i]->BG_Update(player->get_state(), player->get_x_pos());
  }
  //set camera
  SetCamera(); 
  //std::cout << "exit update mechanics" << std::endl;
}


void GameEngine::SetCamera(){
  //set the camera to follow the player and be centered on the middle of the screen
  camera.x = (player->get_x_pos() + player->get_width()/2) - SCREEN_WIDTH/2;
  camera.y = /*(player->get_y_pos() + player->get_height()/2) -*/ SCREEN_HEIGHT/2; 
  //make sure camera doesn't go out of bounds of the level
  if(camera.x < 0){ 
    camera.x = 0;
  }
  if(camera.y < 0){
    camera.y = 0;
  }
  if(camera.x  > LEVEL_WIDTH - camera.w){
    camera.x = LEVEL_WIDTH - camera.w;  }
  if(camera.y > LEVEL_HEIGHT - camera.h){
    camera.y = LEVEL_HEIGHT - camera.h;
  }
}

void GameEngine::SetTiles(){
  //set up tiles locations by reading in map and making the tile objects
  int x = 0, y = 416;
  std::ifstream map("tiles.map");
  if(map.fail()){
    std::cout << "Unable to load map file" << std::endl;
    return;
  }
  else{
    for(int i = 0; i < TOTAL_TILES; i++){
      int tile_type_temp = -1;
      map >> tile_type_temp;

      if((tile_type_temp >= 0) && (tile_type_temp < TOTAL_TILES_SPRITES)){
	Tiles[i] = new Tile(game_renderer);
	Tiles[i]->Obj_Init( x, y, tile_type_temp);
	x += TILE_WIDTH;
      }
    }
  }
}

 
void GameEngine::Render(){
  //set background color
  SDL_SetRenderDrawColor(game_renderer, 135, 206, 235, 255);
  //clear screen
  SDL_RenderClear(game_renderer);
  //obj->Obj_Render();for as many objects as there are
  for(int i = Background_Size - 1; i >= 0; i--){ //render background
    BG[i]->BG_Render();
  }
  for(int i = 0; i < TOTAL_TILES; i++){ //render tiles
    Tiles[i]->Obj_Render(camera);
  }
  player->Obj_Render(camera.x, camera.y); //render player
  SDL_RenderPresent(game_renderer); //present game
}

void GameEngine::Quit(){
  SDL_DestroyRenderer(game_renderer);
  SDL_DestroyWindow(game_window);

  IMG_Quit();
  SDL_Quit();
}
