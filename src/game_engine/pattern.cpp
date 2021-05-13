/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#include "pattern.h"

// ------------------------------------------------------------------------- //

Pattern::Pattern(){

  is_ready_ = false;
  commands_ = std::vector<PatternCommand>(0);

}

// ------------------------------------------------------------------------- //

Pattern::~Pattern(){

  commands_.clear();

}

// ------------------------------------------------------------------------- //

void Pattern::addCommand(PatternCommand command){

  commands_.push_back(command);

}

// ------------------------------------------------------------------------- //

void Pattern::set_ready(){

  is_ready_ = true;

}

// ------------------------------------------------------------------------- //

void Pattern::clear(){

  commands_.clear();
  is_ready_ = false;

}

// ------------------------------------------------------------------------- //

bool Pattern::is_ready() const {

  return is_ready_;

}

// ------------------------------------------------------------------------- //

Pattern::PatternCommand::PatternCommand(){

  direction_ = kPatternDirection_North;
  distance = 0.0f;

}

// ------------------------------------------------------------------------- //

Pattern::PatternCommand::PatternCommand(PatternDirection dir, float dist){

  direction_ = dir;
  distance = dist;

}

// ------------------------------------------------------------------------- //
