/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#include "path.h"
#include "pattern.h"
#include "network_game.h"

#include <SDL.h>

// ------------------------------------------------------------------------- //

Path::Path() {

  points_ = std::vector<glm::vec2>(0);
  transformed_points_ = std::vector<SDL_Point>(0);
  number_points_ = 0;
  current_index_ = 0;
  is_ready_ = false;
  direction_ = kDirection_NONE;
  action_ = kAction_NONE;
  action_loops_ = 0;
  current_loops_ = 0;

}

// ------------------------------------------------------------------------- //

Path::~Path() {

  points_.clear();

}

// ------------------------------------------------------------------------- //

void Path::clear(){

  points_.clear();
  number_points_ = 0;
  current_index_ = 0;
  is_ready_ = false;
  direction_ = kDirection_NONE;
  action_ = kAction_NONE;
  current_loops_ = 0;
  action_loops_ = 0;

}

// ------------------------------------------------------------------------- //

void Path::convertPatternToPath(glm::vec2 agent_position, Pattern pattern){

  // Start the path at agent position
  addPoint(agent_position);

  for (uint32_t i = 0; i < pattern.commands_.size(); ++i) {
    glm::vec2 current_pos;
    if (i >= 1) {
      current_pos = points_[i];
    }
    else {
      current_pos = agent_position;
    }

    switch (pattern.commands_[i].direction_) {
    case Pattern::kPatternDirection_North: {
      addPoint(current_pos + (glm::vec2(0.0f, -1.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_NorthEast: {
      addPoint(current_pos + (glm::vec2(1.0f, -1.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_East: {
      addPoint(current_pos + (glm::vec2(1.0f, 0.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_SouthEast: {
      addPoint(current_pos + (glm::vec2(1.0f, 1.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_South: {
      addPoint(current_pos + (glm::vec2(0.0f, 1.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_SouthWest: {
      addPoint(current_pos + (glm::vec2(-1.0f, 1.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_West: {
      addPoint(current_pos + (glm::vec2(-1.0f, 0.0f) * pattern.commands_[i].distance));
      break;
    }
    case Pattern::kPatternDirection_NorthWest: {
      addPoint(current_pos + (glm::vec2(-1.0f, -1.0f) * pattern.commands_[i].distance));
      break;
    }
    default: {
      assert(false);
      break;
    }
    }

  }

}

// ------------------------------------------------------------------------- //

void Path::set_ready(){

  is_ready_ = true;

}

// ------------------------------------------------------------------------- //

void Path::set_direction(Direction direction){

  direction_ = direction;

}

// ------------------------------------------------------------------------- //

void Path::set_action(Action action){

  action_ = action;
  action_loops_ = 0;

}

// ------------------------------------------------------------------------- //

void Path::set_action(Action action, uint32_t loops){

  action_ = action;
  action_loops_ = loops;

}

// ------------------------------------------------------------------------- //

void Path::addPoint(glm::vec2 point){

  points_.push_back(point);
  number_points_++;

}

// ------------------------------------------------------------------------- //

bool Path::is_first(glm::vec2 point) const {

  return point == points_.front();

}

// ------------------------------------------------------------------------- //

bool Path::is_last(glm::vec2 point) const {
  
  return point == points_.back();

}

// ------------------------------------------------------------------------- //

bool Path::is_ready() const {

  return is_ready_;

}

// ------------------------------------------------------------------------- //

void Path::calculateTransformedPoints(/*glm::mat3 view_matrix*/){

  /*uint32_t points_size = number_points_ * 2;
 
  transformed_points_.clear();

  for (uint32_t i = 0; i < number_points_; ++i) {
    glm::vec2 transform = view_matrix * glm::vec3(points_[i], 0);
    transformed_points_[(i * 2) + 0] = transform.x;
    transformed_points_[(i * 2) + 1] = transform.y;
  }*/

  transformed_points_ = std::vector<SDL_Point>(number_points_);

  for (uint32_t i = 0; i < number_points_; ++i) {
    SDL_Point point = { points_[i].x, points_[i].y };
    transformed_points_[i] = point;
  }

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::next_point() {

  current_index_++;
  if (current_index_ >= (int32_t)number_points_) {
    current_index_ = 0;
  }

  return points_.at(current_index_);

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::prev_point(){

  current_index_--;
  if (current_index_ < 0) {
    current_index_ = number_points_ - 1;
  }

  return points_.at(current_index_);

}

// ------------------------------------------------------------------------- //

uint32_t Path::number_points() const {

  return number_points_;

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::first_point() const {

  return points_.front();

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::at_point(uint32_t index) const {

  if (index < number_points_)
    return points_[index];

  return glm::vec2();

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::last_point() const{

  return points_.back();

}

// ------------------------------------------------------------------------- //

glm::vec2 Path::next_target(){

  // Gets next target
  switch (direction_) {
  case Path::kDirection_Forward:
    return next_point();
    break;
  case Path::kDirection_Backward:
    return prev_point();
    break;
  case Path::kDirection_NONE:
    printf("\nError: agent path direction not set");
  default:
    break;
  }

  return glm::vec2();

}

// ------------------------------------------------------------------------- //

bool Path::reached_target(glm::vec2 target){
  switch (action_) {
  case Path::kAction_Straight:
    if (direction_ == Path::kDirection_Forward) {
      return is_first(target);
     /* if (is_first(target)) current_loops_++;

      if (current_loops_ > 0) {
        return true;
      }*/
    }
    if (direction_ == Path::kDirection_Backward) {
      return is_first(target);
    }
    break;
  case Path::kAction_LoopNTimes:
    if (direction_ == Path::kDirection_Forward) {
      if (is_last(target)) current_loops_++;

      if (current_loops_ >= action_loops_) {
        return true;
      }
    }
    if (direction_ == Path::kDirection_Backward) {
      if (is_first(target)) current_loops_++;

      if (current_loops_ >= action_loops_) {
        return true;
      }
    }
    break;
  case Path::kAction_LoopInfinite:

    break;
  default:
    break;
  }

  return false;

}

// ------------------------------------------------------------------------- //

void Path::drawDebug() {

  if (!NetworkGame::instance().game_menus_->debug_mode_) return;

  SDL_SetRenderDrawColor(NetworkGame::instance().renderer_, 0, 255, 255, 255);

  SDL_RenderDrawLines(NetworkGame::instance().renderer_, transformed_points_.data(), number_points_);
  
}

// ------------------------------------------------------------------------- //

void Path::editor(){



}

// ------------------------------------------------------------------------- //
