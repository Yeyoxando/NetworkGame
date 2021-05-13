/*
 *	Author: Diego Ochando Torres
 *  Date: 2020
 *  e-mail: c0022981@my.shu.ac.uk | yeyoxando@gmail.com
 */

#ifndef __BOX_H__
#define __BOX_H__ 

#include <glm.hpp>
#include <vector>
#include <SDL.h>

// ----------------------------------------------------------------------- //

/// @brief: Used to represent areas such as a Collider or agent vision area
class Box {
 public:
  /// @brief: Box constructor.
  Box();
  /// @brief: Box constructor with parameters.
  Box(glm::vec2 size, glm::vec2 position);
    
  /// @brief: Box copy constructor
  Box(const Box& copy);
  /// @brief: Box copy operator
  void operator=(const Box& copy);
    
  /// @brief: Box destructor
  ~Box();

  /**
    * @brief: draws the box
    * @param: color to draw the shape, from 0 to 255
    */
  void drawDebug(glm::vec4 color);

  /// @brief: moves the point of the box depending on the current box center
  void updatePoints();

  /**
    * @brief: calculates the transformed points respect to the camera and world
    * @param: view matrix from the camera
    */
  void calculateTransformedPoints(/*glm::mat3 view_matrix*/);

  /// @result: true if given position is inside box bounds
  bool isPositionInside(glm::vec2 position);

  // ------------------------------ Setters ------------------------------ //

  /// @brief: Set box size
  void set_size(glm::vec2 new_size);
  /// @brief: Set box world position
  void set_position(glm::vec2 new_position);

  // ------------------------------ Getters ------------------------------ //

  /// @result: Box size
  glm::vec2 size() const;
  /// @result: Box position
  glm::vec2 position() const;

 private:
  // --- Shape ---
  /// @brief: Box size
  glm::vec2 size_;
  /// @brief: Box position
  glm::vec2 position_;
  /// @brief: Box world points with its position and size
  std::vector<glm::vec2> world_points_;
    
  // --- Debug ---
  /// @brief: Stores view transformed points calculated at Step to use it later on Draw
  std::vector<SDL_Point> transformed_points_;

};

// ----------------------------------------------------------------------- //

#endif //__BOX_H__
