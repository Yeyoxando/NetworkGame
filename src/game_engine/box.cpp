/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

// ------------------------------------------------------------------------- //

#include "box.h"
#include "network_game.h"

#include <SDL.h>

// ------------------------------------------------------------------------- //

Box::Box(){

  size_ = glm::vec2(0.0f, 0.0f);
  position_ = glm::vec2(0.0f, 0.0f);

  world_points_ = std::vector<glm::vec2>(4);

}

// ------------------------------------------------------------------------- //

Box::Box(const Box & copy){

  size_ = copy.size_;
  position_ = copy.position_;
  world_points_ = copy.world_points_;
  transformed_points_ = copy.transformed_points_;

}

// ------------------------------------------------------------------------- //

Box::Box(glm::vec2 size, glm::vec2 position) {

  size_ = size;
  position_ = position;

	world_points_ = std::vector<glm::vec2>(4);

  updatePoints();

}

// ------------------------------------------------------------------------- //

Box::~Box(){



}

// ------------------------------------------------------------------------- //

void Box::set_size(glm::vec2 new_size){

  size_ = new_size;

}

// ------------------------------------------------------------------------- //

void Box::set_position(glm::vec2 new_position){

  position_ = new_position;

}

// ------------------------------------------------------------------------- //

glm::vec2 Box::size() const {
  
  return size_;

}

// ------------------------------------------------------------------------- //

glm::vec2 Box::position() const {

  return position_;

}

// ------------------------------------------------------------------------- //

void Box::drawDebug(glm::vec4 color){

	SDL_SetRenderDrawColor(NetworkGame::instance().renderer_, color.r, color.g, color.b, color.a);

	SDL_RenderDrawLines(NetworkGame::instance().renderer_, transformed_points_.data(), 5);

}

// ------------------------------------------------------------------------- //

void Box::updatePoints(){

  world_points_[0].x = position_.x + (size_.x / 2.0f);
  world_points_[0].y = position_.y + (size_.y / 2.0f);
  world_points_[1].x = position_.x + (size_.x / 2.0f);
  world_points_[1].y = position_.y - (size_.y / 2.0f);
  world_points_[2].x = position_.x - (size_.x / 2.0f);
  world_points_[2].y = position_.y - (size_.y / 2.0f);
  world_points_[3].x = position_.x - (size_.x / 2.0f);
  world_points_[3].y = position_.y + (size_.y / 2.0f);

}

// ------------------------------------------------------------------------- //

void Box::calculateTransformedPoints(/*glm::mat3 view_matrix*/){

  /*u32 points_size = 4;

  for (u32 i = 0; i < points_size; ++i) {
    glm::vec2 transform = Mat3::MultiplyPoint(view_matrix, world_points_[i]);
    transformed_points_[(i * 2) + 0] = transform.x;
    transformed_points_[(i * 2) + 1] = transform.y;
  }

  transformed_points_[8] = transformed_points_[0];
  transformed_points_[9] = transformed_points_[1];*/


  transformed_points_ = std::vector<SDL_Point>(5);

  SDL_Point point;
	for (uint32_t i = 0; i < 4; ++i) {
		point = { (int)world_points_[i].x, (int)world_points_[i].y };
		transformed_points_[i] = point;
	}
  point = { (int)world_points_[0].x, (int)world_points_[0].y };
  transformed_points_[4] = point;

}

// ------------------------------------------------------------------------- //

void Box::operator=(const Box & copy){

  size_ = copy.size_;
  position_ = copy.position_;
  world_points_ = copy.world_points_;
  transformed_points_ = copy.transformed_points_;

}

// ------------------------------------------------------------------------- //

bool Box::isPositionInside(glm::vec2 position){

  if (position.x > world_points_[2].x && position.x < world_points_[0].x
    && position.y > world_points_[2].y && position.y < world_points_[0].y) {
    return true;
  }

  return false;

}

// ------------------------------------------------------------------------- //
