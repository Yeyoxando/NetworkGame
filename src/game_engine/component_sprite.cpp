/*
*  Author: Diego Ochando Torres
*  Date: 08-11-2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include <glm.hpp>
#include <stdio.h>
#include <string.h>
#include <SDL_image.h>
#include "component_sprite.h"
#include "network_game.h"
#include "game_object.h"

/*struct ExampleAppLog {
	void    AddLog(const char* fmt, ...) IM_FMTARGS(2);
};*/

unsigned char Sprite::default_texture[16] = {
  0xFF, 0xFF, 0xFF, 0xFF, //White
  0x00, 0x00, 0xFF, 0xFF, //Blue
  0xFF, 0x00, 0x00, 0xFF, //Red
  0xFF, 0xFF, 0xFF, 0xFF,  //White
};

//----------------------------- Constructors --------------------------------//
Sprite::Sprite(){
#ifdef DEBUG_
  printf("\n***ERROR*** Do not call Sprite default constructor." \
    "(Needs a GameObject reference). Use instead another constructor");
#endif // DEBUG_
}

Sprite::Sprite(GameObject& gameobject, const Sprite& other){
  id_ = total_components_;
  
  *this = other;
  
}

Sprite::Sprite(GameObject& gameobject, const char* file_name){
  id_ = total_components_;
  component_tag_ = kComponentKind_Sprite;
  gameobject_ = &gameobject;
  pivot_point_ = kPivotPoint_TopLeft;
  pivot_selection_ = 8;

  int width, height;
	texture_ = NetworkGame::instance().tex_loader_->loadTexture(file_name, &width, &height);


  if(texture_ == nullptr){
		printf("\n[ _error_ ] Error in [%s]: Sprite source (%s) doesn't exist. Check name.", __FUNCTION__, file_name);
    origin_ = kSpriteOrigin_Null;
  }
  else{
    origin_ = kSpriteOrigin_Image;
    render_rect_ = {0, 0, 0, 0};
    render_rect_.w = width;
		render_rect_.h = height;
		sub_sprite_rect_ = {-1, -1, -1, -1};
    gameobject_->transform_.size_ = { (float)width, (float)height };
  }
}

Sprite::Sprite(GameObject& gameobject, int width, int height, const unsigned char* buffer){
  id_ = total_components_;
  component_tag_ = kComponentKind_Sprite;
  gameobject_ = &gameobject;
  pivot_point_ = kPivotPoint_TopLeft;
  SDL_Surface* img = SDL_CreateRGBSurfaceWithFormatFrom((void*)buffer, width, height,
                                            32, 4*width, SDL_PIXELFORMAT_RGBA8888);
  if(img == nullptr){
    origin_ = kSpriteOrigin_Null;
  }
  else {
    origin_ = kSpriteOrigin_Buffer;
    texture_ = SDL_CreateTextureFromSurface(NetworkGame::instance().renderer_, img);
    render_rect_ = { 0, 0, 0, 0 };
    render_rect_.w = width;
    render_rect_.h = height;
    sub_sprite_rect_ = { -1, -1, -1, -1 };
  }
}

Sprite::Sprite(GameObject& gameobject, const char* file_name, int x_pos, int y_pos, 
  int x_size, int y_size){

	id_ = total_components_;
	component_tag_ = kComponentKind_Sprite;
	gameobject_ = &gameobject;
	pivot_point_ = kPivotPoint_TopLeft;
	pivot_selection_ = 8;

	int width, height;
	texture_ = NetworkGame::instance().tex_loader_->loadTexture(file_name, &width, &height);


	if (texture_ == nullptr) {
		printf("\n[ _error_ ] Error in [%s]: Sprite source (%s) doesn't exist. Check name.", __FUNCTION__, file_name);
		origin_ = kSpriteOrigin_Null;
	}
	else {
		origin_ = kSpriteOrigin_Image;
		render_rect_ = { 0, 0, x_size, y_size };
		render_rect_.w = x_size;
		render_rect_.h = y_size;
    sub_sprite_rect_ = { x_pos, y_pos, x_size, y_size };
		gameobject_->transform_.size_ = { (float)x_size, (float)y_size };
	}

}

Sprite::~Sprite(){
  
}

//------------------------------------------------------------------------------//

//------------------------------ Initializers ----------------------------------//
/*void Sprite::initFromFile(const char* file_name){
  component_tag_ = kComponentKind_Sprite;
  pivot_point_ = kPivotPoint_Center;

  memset(img_name_, '\0', 128);
  strcpy(img_name_, file_name);

  char src[128];
  memset(src, '\0', sizeof(default_sprite_folder));
  strcpy(src, default_sprite_folder);
  strcat(src, file_name);
  img_ = IMG_Load(src); 
  
  if(img_ == nullptr){
    origin_ = kSpriteOrigin_Null;
  }
  else{
    origin_ = kSpriteOrigin_Image;
    texture_ = SDL_CreateTextureFromSurface(NetworkGame::instance().renderer_, img_);
    render_rect_ = {0, 0, 0, 0};
    render_rect_.w = img_->w;
		render_rect_.h = img_->h;
    sub_sprite_rect_ = {-1, -1, -1, -1};
		gameobject_->transform_.size_ = { (float)img_->w, (float)img_->h };
  }
}*/

void Sprite::initFromBuffer(int width, int height, const unsigned char* buffer){
  component_tag_ = kComponentKind_Sprite;
  pivot_point_ = kPivotPoint_TopLeft;
  SDL_Surface* img = SDL_CreateRGBSurfaceWithFormatFrom((void*)buffer, width, height,
                                            32, 4*width, SDL_PIXELFORMAT_RGBA8888);
  if(img == nullptr){
    origin_ = kSpriteOrigin_Null;
  }
  else{
    origin_ = kSpriteOrigin_Buffer;
    texture_ = SDL_CreateTextureFromSurface(NetworkGame::instance().renderer_, img);
    render_rect_ = {0, 0, 0, 0};
    render_rect_.w = width;
		render_rect_.h = height;
    sub_sprite_rect_ = { -1, -1, -1, -1 };
  }
}

//---------------------------------------------------------------------------//

//-------------------------------- Setters ----------------------------------//
void Sprite::set_pivotPoint(PivotPoint pivot_point){
  pivot_point_ = pivot_point;
}

//---------------------------------------------------------------------------//

//-------------------------------- Getters ----------------------------------//
const int Sprite::width(){
  return render_rect_.w;
}

const int Sprite::height(){
  return render_rect_.h;
}

const PivotPoint Sprite::pivotPoint(){
  return pivot_point_;
}

//---------------------------------------------------------------------------//

//-------------------------------- Methods ----------------------------------//
void Sprite::update() {

}

void Sprite::draw(){
  if (origin_ != kSpriteOrigin_Null) {
    SDL_Rect rect_instance = { 0, 0, 0, 0 };
    rect_instance.x = (int)gameobject_->transform_.position_.x -
      (int)(width() / 2 * gameobject_->transform_.scale_.x);
    rect_instance.y = (int)gameobject_->transform_.position_.y -
      (int)(height() / 2 * gameobject_->transform_.scale_.y);
    rect_instance.w = (int)(render_rect_.w * gameobject_->transform_.scale_.x);
    rect_instance.h = (int)(render_rect_.h * gameobject_->transform_.scale_.y);




    float rotation_degrees = glm::degrees(gameobject_->transform_.rotation_);
    SDL_Point final_pivot;

    switch (pivot_point_) {
    case kPivotPoint_TopLeft: {
      final_pivot = { 0, 0 };
      break;
    }
    case kPivotPoint_Left: {
      final_pivot = { 0, (int)(height() / 2 * gameobject_->transform_.scale_.y) };
      break;
    }
    case kPivotPoint_BottomLeft: {
      final_pivot = { 0, (int)(height() * gameobject_->transform_.scale_.y) };
      break;
    }
    case kPivotPoint_Bottom: {
      final_pivot = { (int)(width() / 2 * gameobject_->transform_.scale_.x), 0 };
      break;
    }
    case kPivotPoint_BottomRight: {
      final_pivot = { (int)(width() * gameobject_->transform_.scale_.x),
                     (int)(height() * gameobject_->transform_.scale_.y) };
      break;
    }
    case kPivotPoint_Right: {
      final_pivot = { (int)(width() * gameobject_->transform_.scale_.x),
                     (int)(height() / 2 * gameobject_->transform_.scale_.y) };
      break;
    }
    case kPivotPoint_TopRight: {
      final_pivot = { (int)(width() * gameobject_->transform_.scale_.x), 0 };
      break;
    }
    case kPivotPoint_Top: {
      final_pivot = { (int)(width() / 2 * gameobject_->transform_.scale_.x), 0 };
      break;
    }
    case kPivotPoint_Center: {
      final_pivot = { (int)(width() / 2 * gameobject_->transform_.scale_.x),
             (int)(height() / 2 * gameobject_->transform_.scale_.y) };
      break;
    }
    default: {
      final_pivot = { 0, 0 };
      break;
    }
    }

    if (sub_sprite_rect_.x != -1) {
      SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture_, &sub_sprite_rect_, &rect_instance,
        rotation_degrees, &final_pivot, SDL_FLIP_NONE);
    }
    else {
      SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture_, NULL, &rect_instance,
        rotation_degrees, &final_pivot, SDL_FLIP_NONE);
    }
  }
}

void Sprite::draw(Transform transform) {
	if (origin_ != kSpriteOrigin_Null) {
		SDL_Rect rect_instance;
		rect_instance.x = (int)transform.position_.x;
		rect_instance.y = (int)transform.position_.y;
		rect_instance.w = (int)(render_rect_.w * transform.scale_.x);
		rect_instance.h = (int)(render_rect_.h * transform.scale_.y);

		float rotation_degrees = glm::degrees(transform.rotation_);
		SDL_Point final_pivot;

		switch (pivot_point_) {
		case kPivotPoint_TopLeft: {
			final_pivot = { 0, 0 };
			break;
		}
		case kPivotPoint_Left: {
			final_pivot = { 0, (int)(height() / 2 * transform.scale_.y) };
			break;
		}
		case kPivotPoint_BottomLeft: {
			final_pivot = { 0, (int)(height() * transform.scale_.y) };
			break;
		}
		case kPivotPoint_Bottom: {
			final_pivot = { (int)(width() / 2 * transform.scale_.x), 0 };
			break;
		}
		case kPivotPoint_BottomRight: {
			final_pivot = { (int)(width() * transform.scale_.x),
							(int)(height() * transform.scale_.y) };
			break;
		}
		case kPivotPoint_Right: {
			final_pivot = { (int)(width() * transform.scale_.x),
							(int)(height() / 2 * transform.scale_.y) };
			break;
		}
		case kPivotPoint_TopRight: {
			final_pivot = { (int)(width() * transform.scale_.x), 0 };
			break;
		}
		case kPivotPoint_Top: {
			final_pivot = { (int)(width() / 2 * transform.scale_.x), 0 };
			break;
		}
		case kPivotPoint_Center: {
			final_pivot = { (int)(width() / 2 * transform.scale_.x),
							(int)(height() / 2 * transform.scale_.y) };
			break;
		}
		default: {
			final_pivot = { 0, 0 };
			break;
		}
		}
    
    if (sub_sprite_rect_.x != -1) {
			SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture_, &sub_sprite_rect_, &rect_instance,
				rotation_degrees, &final_pivot, SDL_FLIP_NONE);
    }
		else {
			SDL_RenderCopyEx(NetworkGame::instance().renderer_, texture_, NULL, &rect_instance,
				rotation_degrees, &final_pivot, SDL_FLIP_NONE);
    }
	}
}

/*void Sprite::inspector() {
	if (gameobject_->getComponent(kComponentKind_Sprite) != nullptr) {

		static const char* possible_positions[] = {
		"Top Left",
		"Left",
		"Bottom Left",
		"Bottom",
		"BottomRight",
		"Right",
		"Top Right",
		"Top",
		"Center"
		};

		pivot_point_ = PivotPoint(pivot_selection_);


		if (ImGui::TreeNode("Sprite")) {
			static char buf[32] = "example_file.png";
			ImGui::Combo("Pivot point", &pivot_selection_, possible_positions, IM_ARRAYSIZE(possible_positions));
			ImGui::InputText("Sprite name", buf, IM_ARRAYSIZE(buf));
			if(ImGui::Button("Change Sprite")){
				initFromFile(buf);
			}


			//This line will be the last one
			ImGui::TreePop();
		}

	}
}*/
