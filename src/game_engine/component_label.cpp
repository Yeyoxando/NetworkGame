/*
*  Author: Diego Ochando Torres
*  Date: 06-11-2018
*  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
*/

#include <string.h>
#include <stdio.h>
#include <SDL.h>

#include "component_label.h"
#include "game_object.h"
#include "network_game.h"

int label_counter = 0;

//----------------------------- Constructors --------------------------------//

Label::Label() {
#ifdef DEBUG_
  printf("\n***ERROR*** Do not call Label default constructor." \
    "(Needs a GameObject reference). Use instead another constructor");
#endif // DEBUG_
}

Label::Label(GameObject& game_object){
  id_ = total_components_;
  component_tag_ = kComponentKind_Label;
  gameobject_ = &game_object;

  memset(font_name_, '\0', 128);
  strcpy(font_name_, kDefault_font);

  if (!TTF_WasInit()) {
    TTF_Init();
  }
  size_ = 1;

  memset(text_, '\0', kLabelMaxLength);

  font_ = TTF_OpenFont(font_name_, size_);
  size_ = 20;
  color_ = glm::vec4(255, 255, 255, 255);
  alignment_ = kTextAlign_Left;
  style_ = kTextStyle_Normal;
  ++label_counter;
}

Label::Label(GameObject& game_object, const Label& other){
  id_ = total_components_;
  component_tag_ = kComponentKind_Label;
  gameobject_ = &game_object;
  memset(text_, '\0', kLabelMaxLength);
  strcpy(text_, other.text_);
  font_ = other.font_;
  size_ = other.size_;
  color_ = other.color_;
  alignment_ = other.alignment_;
  style_ = other.style_;
}

Label::Label(GameObject& game_object, const char* text, const char* font_src,
             int size, const glm::vec4 color){
  id_ = total_components_;
  component_tag_ = kComponentKind_Label;
  gameobject_ = &game_object;

  memset(font_name_, '\0', 128);
  strcpy(font_name_, font_src);

  if(!TTF_WasInit()){
    TTF_Init();
  }
  if(size > 0){
    size_ = size;
  }
  else{
    size_ = 1;
  }

  memset(text_, '\0', kLabelMaxLength);
  strcpy(text_, text);

  char src[128];
  memset(src, '\0', sizeof(kDefault_font_folder));
  strcpy(src, kDefault_font_folder);
  strcat(src, font_src);

  font_ = TTF_OpenFont(src, size_);
  color_ = color;
  alignment_ = kTextAlign_Left;
  style_ = kTextStyle_Normal;
  ++label_counter;
}

Label::~Label(){
  //Closes the font to release its memory
  if(font_ != nullptr){
    TTF_CloseFont(font_);
  }

  --label_counter;
  //If there is no more labels, closes TTF
  if(label_counter == 0){
    TTF_Quit();
  }
}

//------------------------------------------------------------------------------//

//------------------------------ Initializers ----------------------------------//

void Label::init(){
  component_tag_ = kComponentKind_Label;
  if(!TTF_WasInit()){
    TTF_Init();
  }
  memset(text_, '\0', kLabelMaxLength);
  font_ = nullptr;
  size_ = 1;
  color_ = glm::vec4(255, 255, 255, 255);
  alignment_ = kTextAlign_Left;
  style_ = kTextStyle_Normal;
}

void Label::init(const char* text, const char* font_src, int size, const glm::vec4 color){
  component_tag_ = kComponentKind_Label;

  memset(font_name_, '\0', 128);
  strcpy(font_name_, font_src);

  if(!TTF_WasInit()){
    TTF_Init();
  }
  if(size > 0){
    size_ = size;
  }
  else{
    size = 1;
  }

  memset(text_, '\0', kLabelMaxLength);
  strcpy(text_, text);

  char src[128];
  strcpy(src, kDefault_font_folder);
  strcat(src, font_src);

  font_ = TTF_OpenFont(src, size_);
  
  color_ = color;
  alignment_ = kTextAlign_Left;
  style_ = kTextStyle_Normal;
}

//---------------------------------------------------------------------------//

//------------------------------- Setters -----------------------------------//

void Label::set_text(const char* text){
  if(text != NULL){
    memset(text_, '\0', kLabelMaxLength);
    strncpy_s(text_, text, kLabelMaxLength);
  }
}

void Label::set_font(const char* font){
  if(font != NULL){
    TTF_OpenFont(font, size_);
  }
}

void Label::set_size(int size){
  if(size > 0){
    size_ = size;
  }
  else{
    size = 0;
  }
}

void Label::set_color(const glm::vec4 color) {
  color_ = color;
}
void Label::set_color(const char r, const char g, const char b, const char a) {
  color_.r = r;
  color_.g = g;
  color_.b = b;
  color_.a = a;
}

void Label::set_alignment(const TextAlign alignment){
  alignment_ = alignment;
}

void Label::set_style(const TextStyle style){
  TTF_SetFontStyle(font_, style);
}

//---------------------------------------------------------------------------//

//-------------------------------- Getters ----------------------------------//

const char* Label::text(){
  return text_;
}

const TTF_Font* Label::font(){
 return font_;
}

const int Label::size(){
  return size_;
}

const glm::vec4 Label::color() {
  return color_;
}

const TextAlign Label::alignment(){
  return alignment_;
}

const TextStyle Label::style(){
  return style_;
}

//---------------------------------------------------------------------------//

//-------------------------------- Methods ----------------------------------//

void Label::draw(){
  SDL_Surface* message_surf = TTF_RenderText_Blended(font_, text_, {(Uint8)color_.r, (Uint8)color_.g, (Uint8)color_.b});
  SDL_Texture* message = SDL_CreateTextureFromSurface(NetworkGame::instance().renderer_, message_surf);

  SDL_Rect message_rect;
  message_rect.w = strlen(text_) * size_;
  message_rect.h = (int)(size_ * 1.5f);
  message_rect.x = (int)gameobject_->transform_.position_.x;
  message_rect.y = (int)gameobject_->transform_.position_.y;
  if(alignment_ == kTextAlign_Right){
    message_rect.x -= (int)message_rect.w;
  }
  if(alignment_ == kTextAlign_Center){
    message_rect.x -= (int)message_rect.w/ 2;
  }

  SDL_RenderCopy(NetworkGame::instance().renderer_, message, NULL, &message_rect);

  SDL_FreeSurface(message_surf);
  SDL_DestroyTexture(message);
}
/*
void Label::inspector() {
  if (ImGui::TreeNode("Label")) {

    ImGui::TreePop();
  }
}
*/