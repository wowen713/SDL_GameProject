#include "GameObject.h"
#include "player.h"
#include "GameEngine.h"

Player::Player(SDL_Renderer* ren) : GameObject(ren)
{
  x_vel = 0;
  y_vel = 0; 
}

Player::~Player(){}

void Player::Obj_Update(){
  //std::cout << State << std::endl;
  if(State ==  "JUMP" || is_jumping){ //jump state using del_vel metaphore
    if(!is_jumping){
      is_jumping = true; 
      count = total_jump_frames;
      y_vel = -20;
      State = Prev_State;
      Obj_sprite->set_state(5);
      Obj_sprite->set_frames(6);
      Obj_sprite->set_duration(total_jump_frames);
    }
    else if (is_jumping){
      count -= 1;
      y_vel += g;
      if(count == 0){
	is_jumping = false;
      }
    }
  }
  if(State == "RUNLEFT"){ //run left state
    //if(x_vel != MAX_VEL) x_vel -= 1; //for del_vel
    x_pos -= 5;  //pos metophoer
    if(!is_jumping){ //set the sprite to running sprite
      Obj_sprite->set_state(1);
      Obj_sprite->set_frames(8);
      Obj_sprite->set_duration(200);
    }
      Prev_State = State;
  }
  if(State == "RUNRIGHT"){ //run right state
    //if(!is_jumping){
    //if(x_vel != MAX_VEL) x_vel += 1; //for del_vel
    x_pos += 5; //pos metophore
    if(!is_jumping){ //set sprite to running sprite
      Obj_sprite->set_state(1);
      Obj_sprite->set_frames(8);
      Obj_sprite->set_duration(200);
    }
    Prev_State = State;
    //}
  }
  /* if(y_pos >= SCREEN_HEIGHT + 200 && !is_jumping){
     y_vel -= 5;
     is_jumping = true;
     }
     else if(is_jumping){
     if(y_pos <= SCREEN_HEIGHT - Obj_Height && is_jumping){
     y_vel += 5;
     }
     }*/
  
  if(State == "IDLE"){
    if(!is_jumping){ //set sprite to idle sprite
      Obj_sprite->set_state(0);
      Obj_sprite->set_frames(13);
      Obj_sprite->set_duration(200);
    }
  }
  
  //update the x and y positions of the player after the state actions have been handled
  x_pos += x_vel;
  y_pos += y_vel;
  
  if(x_pos + Obj_Width  >= LEVEL_WIDTH){
    x_pos = LEVEL_WIDTH - Obj_Width;
    x_vel = 0;
  }
  if(x_pos <= 0){
    x_pos = 0;
    x_vel = 0;
  }
  if(y_pos <= 0){
    y_pos = 0;
    y_vel = 0;
  }
  if(y_pos + Obj_Height > LEVEL_HEIGHT - 64){
    y_pos = LEVEL_HEIGHT - Obj_Height - 44;
    y_vel = 0;
    is_jumping = false;
  }
  
  obj_rect.x = x_pos; //update the objects rectangle
  obj_rect.y = y_pos;

  frame_rect = Obj_sprite->sprite_update(); //update the player sprite
}

void Player::Obj_Render(int x, int y){
  SDL_Rect temp;
  temp.x = obj_rect.x - x;
  temp.y = obj_rect.y - y;
  temp.w = obj_rect.w;
  temp.h = obj_rect.h;
  if(State == "RUNLEFT"){ //if running left flip the render
    SDL_RenderCopyEx(obj_renderer, obj_texture, &frame_rect, &temp, 0, NULL, SDL_FLIP_HORIZONTAL);
  }
  else if((State == "IDLE" || State == "JUMP") && Prev_State == "RUNLEFT"){
    //if switches to idle or jump from left keep the flip
    SDL_RenderCopyEx(obj_renderer, obj_texture, &frame_rect, &temp, 0, NULL, SDL_FLIP_HORIZONTAL);
  }
  else{ //render normally
    SDL_RenderCopyEx(obj_renderer, obj_texture, &frame_rect, &temp, 0, NULL, SDL_FLIP_NONE);
  }
}

void Player::set_state(string A){
  State = A; //sets the state so that it can be changed by the key presses
}

string Player::get_state(){
  return State;  //returns state
}

void Player::set_prev_state(string A){
  Prev_State = A; //store previous state before jumping so that we can return to it
}

bool Player::get_is_jumping(){
  return is_jumping; //returns true if the player is mid jump
}

string Player::get_prev_state(){
  return Prev_State;
}
